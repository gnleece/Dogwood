#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

class Primitive
{
public:
    virtual void Render();
    virtual void Cleanup();

protected:
    void Init(GLuint shaderProgram);

    GLuint      m_vao;
    GLuint      m_vbo;
    GLuint      m_ebo;

    GLint       m_positionAttrib;
    GLint       m_colourAttrib;

    GLfloat*    m_vertexData;
    GLsizei     m_vertexDataCount;
    
    GLuint*     m_elementData;
    GLsizei     m_elementDataCount;
};

class Cube : public Primitive
{
public:
    Cube(GLuint shaderProgram);
};

class Triangle : public Primitive
{
public:
    Triangle(GLuint shaderProgram);
};

class Sphere : public Primitive
{
};

class Cylinder : public Primitive
{
};