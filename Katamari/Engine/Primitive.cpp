#include "Primitive.h"

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

void Primitive::Init(GLuint shaderProgram)
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_vertexDataCount, m_vertexData, GL_STATIC_DRAW);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*m_elementDataCount, m_elementData, GL_STATIC_DRAW);

    m_positionAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(m_positionAttrib);
    glVertexAttribPointer(m_positionAttrib, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);

    m_colourAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(m_colourAttrib);
    glVertexAttribPointer(m_colourAttrib, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
}

void Primitive::Render()
{
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glDrawElements(GL_TRIANGLES, m_elementDataCount, GL_UNSIGNED_INT, 0);
}

void Primitive::Cleanup()
{
    glDisableVertexAttribArray(m_positionAttrib);
    glDisableVertexAttribArray(m_colourAttrib);

    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}


GLfloat cube_vertexData[] =
{
    -1.0f,-1.0f,-1.0f, 1.0f, 0.0f, 0.0f, // 0
    -1.0f,-1.0f, 1.0f, 1.0f, 0.5f, 0.0f, // 1
    -1.0f, 1.0f,-1.0f, 1.0f, 0.1f, 0.0f, // 2
    -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.5f, // 3
    1.0f,-1.0f,-1.0f, 1.0f, 0.1f, 0.1f, // 4
    1.0f,-1.0f, 1.0f, 1.0f, 0.0f, 0.1f, // 5
    1.0f, 1.0f,-1.0f, 1.0f, 0.5f, 0.5f, // 6
    1.0f, 1.0f, 1.0f, 0.0f, 0.1f, 0.1f  // 7
};

GLuint cube_elementData[] =
{
    0, 1, 3,
    6, 0, 2,
    5, 0, 4,
    6, 4, 0,
    0, 3, 2,
    5, 1, 0,
    3, 1, 5,
    7, 4, 6,
    4, 7, 5,
    7, 6, 2,
    7, 2, 3,
    7, 3, 5
};

Cube::Cube(GLuint shaderProgram)
{
    m_elementDataCount = 36;
    m_vertexDataCount = 8*6;

    m_vertexData = cube_vertexData;
    m_elementData = cube_elementData;

    Init(shaderProgram);
}

GLfloat triangle_vertexData[] = 
{
    0.0f,  0.5f, 0.f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.f, 0.0f, 0.0f, 1.0f
};

GLuint triangle_elementData[] =
{
    0, 1, 2
};

Triangle::Triangle(GLuint shaderProgram)
{
    m_elementDataCount = 3;
    m_vertexDataCount = 3*6;

    m_vertexData = triangle_vertexData;
    m_elementData = triangle_elementData;

    Init(shaderProgram);
}