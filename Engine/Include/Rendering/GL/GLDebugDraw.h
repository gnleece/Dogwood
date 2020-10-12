#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "Rendering/DebugDraw.h"

class GLMaterial;
class GLRenderManager;
class GLShaderProgram;

class DebugPrimitive
{
public:
    virtual void    Draw(Matrix4x4& transform, ColorRGB& color, bool useDepth = true);

protected:
    GLRenderManager*  m_renderManager;

    GLuint          m_positionBufferID;
    GLuint          m_vertexArrayID;
    GLuint          m_ebo;
    Vector3*        m_positionBufferData;
    GLuint*         m_indices;
    GLShaderProgram*  m_shader;

    int             m_numIndices;
};

class DebugSphere : public DebugPrimitive
{
public:
    ~DebugSphere();
    void            Init(GLRenderManager* renderManager, float radius, int divisions);
};

class DebugCube : public DebugPrimitive
{
public:
    ~DebugCube();
    void            Init(GLRenderManager* renderManager);
};

class DebugCapsule : public DebugPrimitive
{
public:
    ~DebugCapsule();
    void            Init(GLRenderManager* renderManager, float radius, float height, int divisions, eAXIS axis);
    virtual void    Draw(Matrix4x4& transform, ColorRGB& color, bool useDepth = true);

private:
    eAXIS           m_axis;
};

struct Pyramid
{
public:
    void            Init(GLRenderManager* renderManager, float base, float height);
    void            Draw(Matrix4x4& transform, ColorRGB& color);

private:
    GLuint          m_positionBufferID;
    GLuint          m_vertexArrayID;
    GLuint          m_ebo;
    Vector3         m_positionBufferData[4];
    GLuint          m_indices[4 * 3];           // 4 triangles * 3 points per triangle
    GLShaderProgram*  m_shader;
};

struct Gnomon
{
public:
    void            Init(GLRenderManager* renderManager, float arrowBase = 0.1f, float arrowHeight = 0.2f);
    void            Draw(Transform& transform);
    Transform       GetScaledTransform(Transform& transform);       // TODO ugh, hacks

private:
    GLuint          m_positionBufferID;
    GLuint          m_colorBufferID;
    GLuint          m_vertexArrayID;
    Vector3         m_positionBufferData[3 * 2];    // 3 lines * 2 points per line
    ColorRGB        m_colorBufferData[3 * 2];
    GLShaderProgram*  m_shader;

    Pyramid         m_arrow;
    Matrix4x4       m_arrowTransforms[3];
};

class GLDebugDraw : public DebugDraw
{
public:
    friend class DebugDraw;
    friend class RenderManager;

    virtual void DrawLine(Vector3& a, Vector3& b, ColorRGB& Color);
    virtual void RenderLines();

    virtual void DrawSphere(Matrix4x4& transform, ColorRGB color, bool useDepth = true);
    virtual void DrawCube(Matrix4x4& transform, ColorRGB color, bool useDepth = true);
    virtual void DrawCapsule(Matrix4x4& transform, ColorRGB color, bool useDepth = true);

    //void PrepareLineBuffer(Vector3* buffer, int count, GLuint &vao, GLuint &vbo);
    //void DrawLineBuffer(GLuint vao, GLuint vbo, Vector3* buffer, int size, ColorRGB color);

    void Startup(GLRenderManager* renderManager);
    void Shutdown();

    GLMaterial* GetDebugMaterial();

private:
    void SetupDebugMat(GLRenderManager* renderManager);

    GLRenderManager*  m_renderManager;
    GLMaterial*       m_material;
    GLShaderProgram*  m_shader;

    DebugSphere     m_debugSphere;
    DebugCube       m_debugCube;
    DebugCapsule    m_debugCapsule;

    GLuint          m_vertexBufferID;
    GLuint          m_ColorBufferID;
    GLuint          m_vertexArrayID;
    Vector3         m_vertexBufferData[MAX_LINES_PER_FRAME * 2];
    ColorRGB        m_lineColors[MAX_LINES_PER_FRAME * 2];
    int             m_numLines = 0;
};