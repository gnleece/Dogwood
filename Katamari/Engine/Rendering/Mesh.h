#pragma once

#include "..\Math\Algebra.h"
#include "ShaderProgram.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <vector>

class Material;

class Mesh
{
public:
    Mesh(std::string filename);

    void Render(Matrix4x4& transform, Material* material);
    void Delete();

private:
    GLint       m_uniModel;

    GLuint      m_vao;
    GLuint      m_vboPosition;
    GLuint      m_vboNormal;
    GLuint      m_vboUV;
    GLuint      m_ebo;

    GLfloat*    m_vertexPositionData;
    GLfloat*    m_vertexNormalData;
    GLfloat*    m_vertexUVData;
    GLsizei     m_vertexCount;

    GLuint*     m_elementData;
    GLsizei     m_elementDataCount;

    GLenum      m_drawMode;

    std::vector<Vector3> positions; // TODO don't need to store these
    std::vector<Vector3> normals;
    std::vector<Vector2> uvs;
    std::vector<GLuint>  indices;
};