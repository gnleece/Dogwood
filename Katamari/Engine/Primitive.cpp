#include "Primitive.h"

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

// TODO (gnleece) OPENGL: use Vertex Buffer Objects instead of CallLists?

Primitive::Primitive() : m_compiled(false)
{ }

void Sphere::Compile()
{
    if (!m_compiled)
    {
        m_listID = glGenLists(1);
        glNewList(m_listID, GL_COMPILE);
        {
            GLUquadricObj* quad;
            quad = gluNewQuadric();
            gluSphere(quad, 1.0, 20, 20);
            gluDeleteQuadric(quad);
        }
        glEndList();
        m_compiled = true;
    }
}

void Sphere::Render()
{
    glCallList(m_listID);
}