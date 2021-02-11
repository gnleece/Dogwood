#pragma once

#ifdef GRAPHICS_GL

#include "Testing/GraphicsAPI.h"
#include "Testing/GL/GLGraphicsAPI.h"

class GLGraphicsAPI : public GraphicsAPI
{
public:
    static GLGraphicsAPI* Create();

    virtual string GetGraphicsAPIName();
};

#endif