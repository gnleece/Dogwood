#pragma once

#include "..\Math\Transform.h"
#include "..\Scene\Resource.h"
#include "ShaderProgram.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <vector>

class Material;
class ResourceInfo;

class Mesh : public Resource
{
public:
    Mesh(std::string filename, ResourceInfo* resourceInfo);

    void    Render(Transform& transform, Material* material, bool wireframe = false);
    void    Delete();

    float   GetBoundingRadius();

    int     GetTriangleCount();
    void    GetTriangle(int index, Vector3* triangle);

private:
    void    CalculateBoundingRadius(std::vector<Vector3>& vertices);

    GLuint      m_vao;
    GLuint      m_vboPosition;
    GLuint      m_vboNormal;
    GLuint      m_vboUV;
    GLuint      m_ebo;

    GLsizei     m_vertexCount;
    GLsizei     m_indexedVertexCount;
    bool        m_hasUVs;

    std::vector<Vector3> m_positions;
    std::vector<GLuint>  m_indices;

    GLenum      m_drawMode;
    float       m_boundingRadius;
};