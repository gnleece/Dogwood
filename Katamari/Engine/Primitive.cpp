#include "Primitive.h"

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "Image.h"
#include "Model.h"
#include "Texture.h"

void Primitive::Init(const ShaderProgram & shaderProgram)
{
    m_shaderProgramID = shaderProgram.GetID();

    // prepare buffers
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vboPosition);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboPosition);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_vertexCount*3, m_vertexPositionData, GL_STATIC_DRAW);
    
    glGenBuffers(1, &m_vboNormal);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboNormal);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_vertexCount*3, m_vertexNormalData, GL_STATIC_DRAW);

    glGenBuffers(1, &m_vboUV);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboUV);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_vertexCount*2, m_vertexUVData, GL_STATIC_DRAW);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*m_elementDataCount, m_elementData, GL_STATIC_DRAW);

    m_positionAttrib = shaderProgram.GetAttribLocation(ShaderProgram::ATTRIB_POS);
    glEnableVertexAttribArray(m_positionAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboPosition);
    glVertexAttribPointer(m_positionAttrib, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
 
    m_normalAttrib = shaderProgram.GetAttribLocation(ShaderProgram::ATTRIB_NORMAL);
    glEnableVertexAttribArray(m_normalAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboNormal);
    glVertexAttribPointer(m_normalAttrib, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);

    m_texAttrib = shaderProgram.GetAttribLocation(ShaderProgram::ATTRIB_TEXCOORD);
    glEnableVertexAttribArray(m_texAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboUV);
    glVertexAttribPointer(m_texAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    
    glUseProgram(m_shaderProgramID);
    m_uniModel = glGetUniformLocation(m_shaderProgramID, "model");
    m_uniColour = glGetUniformLocation(m_shaderProgramID, "materialColor");
}

void Primitive::Render()
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

    glUniformMatrix4fv(m_uniModel, 1, GL_FALSE, m_transform.Transpose().Start());
    glUniform3fv(m_uniColour, 1, m_colour.Start());
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glDrawElements(m_drawMode, m_elementDataCount, GL_UNSIGNED_INT, 0);
}

void Primitive::Cleanup()
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

Cube::Cube(const ShaderProgram & shaderProgram)
{
    Model cubeModel("Engine\\Assets\\Models\\cube.obj");
    m_vertexPositionData = cubeModel.vertices[0].Start();
    m_vertexNormalData = cubeModel.normals[0].Start();
    m_vertexUVData = cubeModel.uvs[0].Start();

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

GLfloat triangle_UVdata[] =
{
    0.5f, 0.5f,
    1.0f, 1.0f,
    0.f, 1.0f
};

GLfloat triagnle_normalData[] =
{
    0.0f, 0.0f, 1.f,
    0.0f, 0.0f, 1.f,
    0.0f, 0.0f, 1.f,
};

GLuint triangle_elementData[] =
{
    0, 1, 2
};

Triangle::Triangle(const ShaderProgram & shaderProgram)
{
    m_vertexPositionData = triangle_vertexData;
    m_vertexNormalData = triagnle_normalData;
    m_vertexUVData = triangle_UVdata;
    m_vertexCount = 3;

    m_elementData = triangle_elementData;
    m_elementDataCount = 3;

    m_drawMode = GL_TRIANGLES;

    Init(shaderProgram);
}

Sphere::Sphere(const ShaderProgram & shaderProgram)
{
    Model sphereModel("Engine\\Assets\\Models\\sphere.obj");
    m_vertexPositionData = sphereModel.vertices[0].Start();
    m_vertexNormalData = sphereModel.normals[0].Start();
    m_vertexUVData = sphereModel.uvs[0].Start();

    m_vertexCount = sphereModel.vertices.size();

    // TODO fix this properly
    m_elementData = new GLuint[m_vertexCount];
    for (int i = 0; i < m_vertexCount; i++)
    {
        m_elementData[i] = i;
    }
    m_elementDataCount = m_vertexCount;

    m_drawMode = GL_TRIANGLES;

    Init(shaderProgram);
}