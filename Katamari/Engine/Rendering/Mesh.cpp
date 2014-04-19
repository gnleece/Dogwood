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
    LoadIndexedModel(path, positions, normals, uvs, indices);
    m_vertexPositionData = positions[0].Start();
    m_vertexNormalData = normals[0].Start();
    m_vertexUVData = uvs[0].Start();

    m_vertexCount = positions.size();

    m_elementData = indices.data();
    m_elementDataCount = indices.size();

    m_drawMode = GL_TRIANGLES;

    // bind buffer data
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vboPosition);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboPosition);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_vertexCount * 3, m_vertexPositionData, GL_STATIC_DRAW);

    glGenBuffers(1, &m_vboNormal);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboNormal);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_vertexCount * 3, m_vertexNormalData, GL_STATIC_DRAW);

    glGenBuffers(1, &m_vboUV);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboUV);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_vertexCount * 2, m_vertexUVData, GL_STATIC_DRAW);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*m_elementDataCount, m_elementData, GL_STATIC_DRAW);
}

void Mesh::Render(Matrix4x4& transform, Material* material)
{
    if (material)
    {
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

        material->ApplyMaterial(m_vboPosition, m_vboNormal, m_vboUV, transform);
        glDrawElements(m_drawMode, m_elementDataCount, GL_UNSIGNED_INT, 0);
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