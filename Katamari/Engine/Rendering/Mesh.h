#pragma once

#include "..\Math\Transform.h"
#include "ShaderProgram.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <vector>

class Material;

class Mesh
{
public:
    Mesh(std::string filename);

    void Render(Transform& transform, Material* material);
    void Delete();

private:
    GLuint      m_vao;
    GLuint      m_vboPosition;
    GLuint      m_vboNormal;
    GLuint      m_vboUV;
    GLuint      m_ebo;

    GLsizei     m_vertexCount;
    GLsizei     m_indexedVertexCount;

    GLenum      m_drawMode;
};