#include "Primitive.h"

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "Image.h"
#include "Texture.h"

void Primitive::Init(GLuint shaderProgram)
{
    m_shaderProgram = shaderProgram;
    
    // prepare buffers
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vboPosition);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboPosition);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_vertexCount*3, m_vertexPositionData, GL_STATIC_DRAW);

    glGenBuffers(1, &m_vboUV);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboUV);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_vertexCount*2, m_vertexUVData, GL_STATIC_DRAW);

    glGenBuffers(1, &m_vboColour);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboColour);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_vertexCount*3, m_vertexColourData, GL_STATIC_DRAW);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*m_elementDataCount, m_elementData, GL_STATIC_DRAW);

    // get shader param locations   TODO this should be stored with the shader itself
    m_positionAttrib = glGetAttribLocation(m_shaderProgram, "position");
    glEnableVertexAttribArray(m_positionAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboPosition);
    glVertexAttribPointer(m_positionAttrib, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
 
    m_colourAttrib = glGetAttribLocation(m_shaderProgram, "color");
    glEnableVertexAttribArray(m_colourAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboColour);
    glVertexAttribPointer(m_colourAttrib, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);

    m_UVAttrib = glGetAttribLocation(m_shaderProgram, "texcoord");
    glEnableVertexAttribArray(m_UVAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboUV);
    glVertexAttribPointer(m_UVAttrib, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);

    Texture tex("Engine\\Assets\\test_texture.bmp");
    
    glUseProgram(m_shaderProgram);
    m_uniModel = glGetUniformLocation(m_shaderProgram, "model");
}

void Primitive::Render()
{
    glUseProgram(m_shaderProgram);      // TODO check if shader is already active
    glUniformMatrix4fv(m_uniModel, 1, GL_FALSE, m_transform.Transpose().Start());
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

GLfloat line_UVData[] =
{
    0.0f, 0.0f,
    1.0f, 0.0f
};

GLuint line_elementData[] =
{
    0, 1
};

Line::Line(GLuint shaderProgram)
{
    m_vertexPositionData = line_vertexData;
    m_vertexUVData = line_UVData;
    m_vertexColourData = line_colourData;
    m_vertexCount = 2;

    m_elementData = line_elementData;
    m_elementDataCount = 2;
    
    m_drawMode = GL_LINE_STRIP;

    Init(shaderProgram);
}

GLfloat cube_vertexData[] =
{
    0.0f, 0.0f, 0.0f, // 0
    0.0f, 0.0f, 1.0f, // 1
    0.0f, 1.0f, 1.0f, // 3

    1.0f, 1.0f, 0.0f, // 6
    0.0f, 0.0f, 0.0f, // 0
    0.0f, 1.0f, 0.0f, // 2

    1.0f, 0.0f, 1.0f, // 5
    0.0f, 0.0f, 0.0f, // 0
    1.0f, 0.0f, 0.0f, // 4

    1.0f, 1.0f, 0.0f, // 6
    1.0f, 0.0f, 0.0f, // 4
    0.0f, 0.0f, 0.0f, // 0

    0.0f, 0.0f, 0.0f, // 0
    0.0f, 1.0f, 1.0f, // 3
    0.0f, 1.0f, 0.0f, // 2

    1.0f, 0.0f, 1.0f, // 5
    0.0f, 0.0f, 1.0f, // 1
    0.0f, 0.0f, 0.0f, // 0

    0.0f, 1.0f, 1.0f, // 3
    0.0f, 0.0f, 1.0f, // 1
    1.0f, 0.0f, 1.0f, // 5

    1.0f, 1.0f, 1.0f, // 7
    1.0f, 0.0f, 0.0f, // 4
    1.0f, 1.0f, 0.0f, // 6

    1.0f, 0.0f, 0.0f, // 4
    1.0f, 1.0f, 1.0f, // 7
    1.0f, 0.0f, 1.0f, // 5

    1.0f, 1.0f, 1.0f, // 7
    1.0f, 1.0f, 0.0f, // 6
    0.0f, 1.0f, 0.0f, // 2

    1.0f, 1.0f, 1.0f, // 7
    0.0f, 1.0f, 0.0f, // 2
    0.0f, 1.0f, 1.0f, // 3

    1.0f, 1.0f, 1.0f, // 7
    0.0f, 1.0f, 1.0f, // 3
    1.0f, 0.0f, 1.0f, // 5
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
    0.0f, 0.1f, 0.1f,
    1.0f, 0.0f, 0.0f,

    1.0f, 0.5f, 0.0f,
    1.0f, 0.1f, 0.0f,
    1.0f, 0.0f, 0.5f,

    1.0f, 0.1f, 0.1f,
    1.0f, 0.0f, 0.1f,
    1.0f, 0.5f, 0.5f,

    0.0f, 0.1f, 0.1f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.5f, 0.0f,

    1.0f, 0.1f, 0.0f,
    1.0f, 0.0f, 0.5f,
    1.0f, 0.1f, 0.1f,

    1.0f, 0.0f, 0.1f,
    1.0f, 0.5f, 0.5f,
    0.0f, 0.1f, 0.1f,

    1.0f, 0.1f, 0.1f,
    1.0f, 0.0f, 0.1f,
    1.0f, 0.5f, 0.5f,

    0.0f, 0.1f, 0.1f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.5f, 0.0f,

    1.0f, 0.1f, 0.0f,
    1.0f, 0.0f, 0.5f,
    1.0f, 0.1f, 0.1f,

    1.0f, 0.0f, 0.1f,
    1.0f, 0.5f, 0.5f,
    0.0f, 0.1f, 0.1f,

};

GLfloat cube_UVData[] = {
    1.0f, 1.0f,
    0.0f, 1.0f, 
    0.0f, 0.0f,

    1.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,

    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,

    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,

    1.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,

    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,

    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,

    1.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,

    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,

    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,

    1.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,

    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f
};

GLuint cube_elementData[] =
{
    0, 1, 2,
    3, 4, 5,
    6, 7, 8,
    9, 10, 11,
    12, 13, 14,
    15, 16, 17,
    18, 19, 20,
    21, 22, 23,
    24, 25, 26,
    27, 28, 29,
    30, 31, 32,
    33, 34, 35
};

Cube::Cube(GLuint shaderProgram)
{
    m_vertexPositionData = cube_vertexData;
    m_vertexColourData = cube_colourData;
    m_vertexUVData = cube_UVData;
    m_vertexCount = 36;

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

GLfloat triangle_UVdata[] =
{
    0.5f, 0.5f,
    1.0f, 1.0f,
    0.f, 1.0f
};

GLuint triangle_elementData[] =
{
    0, 1, 2
};

Triangle::Triangle(GLuint shaderProgram)
{
    m_vertexPositionData = triangle_vertexData;
    m_vertexColourData = triangle_colourData;
    m_vertexUVData = triangle_UVdata;
    m_vertexCount = 3;

    m_elementData = triangle_elementData;
    m_elementDataCount = 3;

    m_drawMode = GL_TRIANGLES;

    Init(shaderProgram);
}