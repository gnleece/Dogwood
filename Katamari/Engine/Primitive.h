#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "Math\Algebra.h"
#include "Colour.h"
#include "ShaderProgram.h"

class Texture;

class Primitive
{
public:
    virtual void Render();
    virtual void Cleanup();

    void SetTransform(Matrix4x4 transform)  { m_transform = transform; }
    void SetTexture(Texture* texture)       { m_texture = texture; }
    void SetColour(ColourRGB colour)        { m_colour = colour; }

protected:
    void Init(const ShaderProgram & shaderProgram);

    Matrix4x4   m_transform;        // TODO this should go in SceneNode
    Texture*    m_texture;          // TODO this should go in SceneNode
    ColourRGB   m_colour;           // TODO this should be per material?

    GLint       m_uniModel;
    GLint       m_uniColour;

    GLuint      m_vao;
    GLuint      m_vboPosition;
    GLuint      m_vboNormal;
    GLuint      m_vboUV;
    GLuint      m_ebo;

    GLuint      m_shaderProgramID;
    GLint       m_positionAttrib;
    GLint       m_normalAttrib;
    GLint       m_texAttrib;

    GLfloat*    m_vertexPositionData;
    GLfloat*    m_vertexNormalData;
    GLfloat*    m_vertexUVData;
    GLsizei     m_vertexCount;

    GLuint*     m_elementData;
    GLsizei     m_elementDataCount;

    GLenum      m_drawMode;
};

class Cube : public Primitive
{
public:
    Cube(const ShaderProgram & shaderProgram);
};

class Triangle : public Primitive
{
public:
    Triangle(const ShaderProgram & shaderProgram);
};

class Sphere : public Primitive
{
public:
    Sphere(const ShaderProgram & shaderProgram);
};

class Cylinder : public Primitive
{
};