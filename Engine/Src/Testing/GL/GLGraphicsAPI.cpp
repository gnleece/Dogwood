#include "Testing/GraphicsAPI.h"
#include "Testing/GL/GLGraphicsAPI.h"

GraphicsAPI* GraphicsAPI::Create()
{
    return GLGraphicsAPI::Create();
}

GLGraphicsAPI* GLGraphicsAPI::Create()
{
    return new GLGraphicsAPI();
}

string GLGraphicsAPI::GetGraphicsAPIName()
{
    return "OpenGL";
}