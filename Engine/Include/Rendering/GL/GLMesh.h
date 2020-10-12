#pragma once

#ifdef GRAPHICS_GL

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "Rendering/Mesh.h"

class GLMesh : public Mesh
{
public:
    friend class Mesh;

    virtual void    Init(std::string filename, ResourceInfo* resourceInfo);

    virtual void    Render(Transform& transform, Material* material, bool wireframe = false);
    virtual void    Delete();

    virtual float   GetBoundingRadius();

    virtual int     GetTriangleCount();
    virtual void    GetTriangle(int index, Vector3* triangle);

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

#endif