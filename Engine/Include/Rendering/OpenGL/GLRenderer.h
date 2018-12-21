#pragma once

#include "Rendering/RenderManager.h"

class GLRenderer : public PlatSpecificRenderer
{
    void ApplyLight(Light& light, ShaderProgram* shader);
};