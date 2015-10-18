#include "Rendering\Mesh.h"

#include "Debugging\DebugDraw.h"
#include "Rendering\Image.h"
#include "Rendering\Material.h"
#include "Rendering\ModelLoading.h"

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

// TODO For a given mesh, we should do the indexing and bounding sphere calculations once when
// the mesh is imported to a project, instead of having to do it at load time every time

Mesh::Mesh(std::string path, ResourceInfo* resourceInfo)
{
    m_drawMode = GL_TRIANGLES;
    m_resourceInfo = resourceInfo;

    std::vector<Vector3> positions;
    std::vector<Vector3> normals;
    std::vector<Vector2> uvs;
    std::vector<GLuint>  indices;

    LoadIndexedModel(path, positions, normals, uvs, indices);
    CalculateBoundingRadius(positions);

    m_vertexCount = positions.size();
    m_indexedVertexCount = indices.size();
    m_hasUVs = uvs.size() > 0;

    // Bind buffer data
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // Upload vertex data: Positions
    glGenBuffers(1, &m_vboPosition);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboPosition);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_vertexCount * 3, positions[0].Start(), GL_STATIC_DRAW);

    // Upload vertex data: Normals
    glGenBuffers(1, &m_vboNormal);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboNormal);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_vertexCount * 3, normals[0].Start(), GL_STATIC_DRAW);

    // Upload vertex data: UVs (if applicable)
    if (m_hasUVs)
    {
        glGenBuffers(1, &m_vboUV);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboUV);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_vertexCount * 2, uvs[0].Start(), GL_STATIC_DRAW);
    }

    // Upload vertex data: Element buffer (for indexing)
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*m_indexedVertexCount, indices.data(), GL_STATIC_DRAW);
}

void Mesh::Render(Transform& transform, Material* material, bool wireframe)
{
    if (material)
    {
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

        material->ApplyMaterial(transform, m_vboPosition, m_vboNormal, m_vboUV, m_hasUVs);
        glDrawElements(m_drawMode, m_indexedVertexCount, GL_UNSIGNED_INT, 0);
        material->UnapplyMaterial();

        if (wireframe)
        {
            Material* debugMat = DebugDraw::Singleton().GetDebugMaterial();
            debugMat->SetColor("color", ColorRGB(0.7f,0.7f,0.7f));
            debugMat->ApplyMaterial(transform, m_vboPosition, m_vboNormal, m_vboUV, m_hasUVs);
            glDrawElements(GL_LINE_LOOP, m_indexedVertexCount, GL_UNSIGNED_INT, 0);
            debugMat->UnapplyMaterial();
        }
    }
}

void Mesh::Delete()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vboPosition);
    glDeleteBuffers(1, &m_vboNormal);
    if (m_hasUVs)
    {
        glDeleteBuffers(1, &m_vboUV);
    }
    glDeleteBuffers(1, &m_ebo);
}

float Mesh::GetBoundingRadius()
{
    return m_boundingRadius;
}

void Mesh::CalculateBoundingRadius(std::vector<Vector3>& vertices)
{
    // Find the vertex with the max distance from the center of the object
    // This will be the radius of the bound sphere
    float max = 0;
    std::vector<Vector3>::iterator iter;
    for (iter = vertices.begin(); iter != vertices.end(); iter++)
    {
        float distance = (*iter).Magnitude();
        if (distance > max)
        {
            max = distance;
        }
    }
    m_boundingRadius = max;
}