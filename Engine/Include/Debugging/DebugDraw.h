#pragma once

#include "..\Math\Algebra.h"
#include "..\Rendering\Colour.h"

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

class Material;
class ShaderProgram;

class DebugDraw
{
public:
    const static int MAX_LINES_PER_FRAME = 20;  // TODO this is pretty lame. resize dynamically

    static DebugDraw& Singleton()
    {
        static DebugDraw    singleton;
        return singleton;
    }
    DebugDraw() {}

    void            Startup();
    void            Shutdown();

    void            DrawLine(Vector3& a, Vector3& b, ColourRGB& colour);       // draw line for one frame
    void            RenderLines();

    void            PrepareLineBuffer(Vector3* buffer, int count, GLuint &vao, GLuint &vbo);
    void            DrawLineBuffer(GLuint vao, GLuint vbo, Vector3* buffer, int size, ColourRGB color);

    Material*       GetDebugMaterial();

private:
    void            SetupDebugMat();

    GLuint          m_vertexBufferID;
    GLuint          m_colourBufferID;
    GLuint          m_vertexArrayID;
    Vector3         m_vertexBufferData[MAX_LINES_PER_FRAME * 2];
    ColourRGB       m_lineColours[MAX_LINES_PER_FRAME * 2];
    int             m_numLines = 0;

    Material*       m_material;
    ShaderProgram*  m_shader;
};