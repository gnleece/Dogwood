#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "Math\Algebra.h"
#include "Colour.h"

class Texture;

class Primitive
{
public:
    virtual void Render();
    virtual void Cleanup();

    void SetTransform(Matrix4x4 transform)  { m_transform = transform; }
    void SetTexture(Texture* texture)       { m_texture = texture; }
    void SetColour(ColourRGB colour);

protected:
    void Init(GLuint shaderProgram);

    Matrix4x4   m_transform;        // TODO this should go in SceneNode
    Texture*    m_texture;          // TODO this should go in SceneNode
    ColourRGB   m_colour;

    GLuint      m_shaderProgram;
    GLint       m_uniModel;

    GLuint      m_vao;
    GLuint      m_vboPosition;
    GLuint      m_vboColour;
    GLuint      m_vboUV;
    GLuint      m_ebo;

    GLint       m_positionAttrib;       // TODO store this per shader program not per primitive
    GLint       m_colourAttrib;
    GLint       m_UVAttrib;

    GLfloat*    m_vertexPositionData;
    GLfloat*    m_vertexColourData;
    GLfloat*    m_vertexUVData;
    GLsizei     m_vertexCount;

    GLuint*     m_elementData;
    GLsizei     m_elementDataCount;

    GLenum      m_drawMode;
};

class Line : public Primitive
{
public:
    Line(GLuint shaderProgram);
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