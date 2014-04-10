#include "Mesh.h"

#include "ModelLoading.h"
#include "Image.h"
#include "Texture.h"

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

Mesh::Mesh(std::string path, const ShaderProgram & shaderProgram) : m_texture(NULL)
{
    LoadIndexedModel(path, positions, normals, uvs, indices);
    m_vertexPositionData = positions[0].Start();
    m_vertexNormalData = normals[0].Start();
    m_vertexUVData = uvs[0].Start();

    m_vertexCount = positions.size();

    m_elementData = indices.data();
    m_elementDataCount = indices.size();

    m_drawMode = GL_TRIANGLES;

    m_shaderProgramID = shaderProgram.GetID();

    // prepare buffers
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

    m_positionAttrib = shaderProgram.GetAttribLocation(ShaderProgram::ATTRIB_POS);
    glEnableVertexAttribArray(m_positionAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboPosition);
    glVertexAttribPointer(m_positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    m_normalAttrib = shaderProgram.GetAttribLocation(ShaderProgram::ATTRIB_NORMAL);
    glEnableVertexAttribArray(m_normalAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboNormal);
    glVertexAttribPointer(m_normalAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    m_texAttrib = shaderProgram.GetAttribLocation(ShaderProgram::ATTRIB_TEXCOORD);
    glEnableVertexAttribArray(m_texAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboUV);
    glVertexAttribPointer(m_texAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    glUseProgram(m_shaderProgramID);
    m_uniModel = glGetUniformLocation(m_shaderProgramID, "model");
    m_uniColour = glGetUniformLocation(m_shaderProgramID, "materialColor");
}

void Mesh::Render(Matrix4x4& transform)
{
    if (m_texture == NULL)
    {
        m_texture = Texture::DefaultTexture();
    }
    m_texture->BindTexture();

    // enable shader if not already active
    GLint currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&currentProgram);
    if (currentProgram != m_shaderProgramID)
    {
        glUseProgram(m_shaderProgramID);
    }

    glUniformMatrix4fv(m_uniModel, 1, GL_FALSE, transform.Transpose().Start());
    glUniform3fv(m_uniColour, 1, m_colour.Start());
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glDrawElements(m_drawMode, m_elementDataCount, GL_UNSIGNED_INT, 0);
}

void Mesh::Delete()
{
    glDisableVertexAttribArray(m_positionAttrib);
    glDisableVertexAttribArray(m_normalAttrib);
    glDisableVertexAttribArray(m_texAttrib);

    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vboPosition);
    glDeleteBuffers(1, &m_vboNormal);
    glDeleteBuffers(1, &m_vboUV);
    glDeleteBuffers(1, &m_ebo);
}