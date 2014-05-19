#include "Mesh.h"

#include "Image.h"
#include "Material.h"
#include "ModelLoading.h"

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

Mesh::Mesh(std::string path)
{
    m_drawMode = GL_TRIANGLES;

    std::vector<Vector3> positions;
    std::vector<Vector3> normals;
    std::vector<Vector2> uvs;
    std::vector<GLuint>  indices;

    LoadIndexedModel(path, positions, normals, uvs, indices);

    m_vertexCount = positions.size();
    m_indexedVertexCount = indices.size();

    // bind buffer data
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // upload vertex data
    glGenBuffers(1, &m_vboPosition);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboPosition);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_vertexCount * 3, positions[0].Start(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_vboNormal);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboNormal);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_vertexCount * 3, normals[0].Start(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_vboUV);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboUV);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_vertexCount * 2, uvs[0].Start(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*m_indexedVertexCount, indices.data(), GL_STATIC_DRAW);
}

void Mesh::Render(Transform& transform, Material* material)
{
    if (material)
    {
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

        material->ApplyMaterial(m_vboPosition, m_vboNormal, m_vboUV, transform);
        glDrawElements(m_drawMode, m_indexedVertexCount, GL_UNSIGNED_INT, 0);
        material->UnapplyMaterial();
    }
}

void Mesh::Delete()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vboPosition);
    glDeleteBuffers(1, &m_vboNormal);
    glDeleteBuffers(1, &m_vboUV);
    glDeleteBuffers(1, &m_ebo);
}