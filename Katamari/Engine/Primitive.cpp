#include "Primitive.h"

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

void Primitive::Init(GLuint shaderProgram)
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vboPosition);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboPosition);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_vertexCount*3, m_vertexPositionData, GL_STATIC_DRAW);

    glGenBuffers(1, &m_vboColour);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboColour);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_vertexCount*3, m_vertexColourData, GL_STATIC_DRAW);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*m_elementDataCount, m_elementData, GL_STATIC_DRAW);

    m_positionAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(m_positionAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboPosition);
    glVertexAttribPointer(m_positionAttrib, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);

    m_colourAttrib = glGetAttribLocation(shaderProgram, "colour");
    glEnableVertexAttribArray(m_colourAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboColour);
    glVertexAttribPointer(m_colourAttrib, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
}

void Primitive::Render()
{
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glDrawElements(m_drawMode, m_elementDataCount, GL_UNSIGNED_INT, 0);
}

void Primitive::Cleanup()
{
    glDisableVertexAttribArray(m_positionAttrib);
    glDisableVertexAttribArray(m_colourAttrib);

    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vboPosition);
    glDeleteBuffers(1, &m_vboColour);
    glDeleteBuffers(1, &m_ebo);
}


GLfloat line_vertexData[] =
{
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f
};

GLfloat line_colourData[] =
{
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f
};

GLuint line_elementData[] =
{
    0, 1
};

Line::Line(GLuint shaderProgram)
{
    m_vertexPositionData = line_vertexData;
    m_vertexColourData = line_colourData;
    m_vertexCount = 2;

    m_elementData = line_elementData;
    m_elementDataCount = 2;
    
    m_drawMode = GL_LINE_STRIP;

    Init(shaderProgram);
}

GLfloat cube_vertexData[] =
{
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
     1.0f,-1.0f,-1.0f,
     1.0f,-1.0f, 1.0f,
     1.0f, 1.0f,-1.0f,
     1.0f, 1.0f, 1.0f
};

GLfloat cube_colourData[] =
{
    1.0f, 0.0f, 0.0f,
    1.0f, 0.5f, 0.0f,
    1.0f, 0.1f, 0.0f,
    1.0f, 0.0f, 0.5f,
    1.0f, 0.1f, 0.1f,
    1.0f, 0.0f, 0.1f,
    1.0f, 0.5f, 0.5f,
    0.0f, 0.1f, 0.1f
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
    m_vertexPositionData = cube_vertexData;
    m_vertexColourData = cube_colourData;
    m_vertexCount = 8;

    m_elementData = cube_elementData;
    m_elementDataCount = 3*6*2;

    m_drawMode = GL_TRIANGLES;

    Init(shaderProgram);
}

GLfloat triangle_vertexData[] = 
{
     0.0f,  0.5f, 0.f,
     0.5f, -0.5f, 0.f,
    -0.5f, -0.5f, 0.f,
};

GLfloat triangle_colourData[] =
{
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

GLuint triangle_elementData[] =
{
    0, 1, 2
};

Triangle::Triangle(GLuint shaderProgram)
{
    m_vertexPositionData = triangle_vertexData;
    m_vertexColourData = triangle_colourData;
    m_vertexCount = 3;

    m_elementData = triangle_elementData;
    m_elementDataCount = 3;

    m_drawMode = GL_TRIANGLES;

    Init(shaderProgram);
}