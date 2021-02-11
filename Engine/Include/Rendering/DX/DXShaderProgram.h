#pragma once

#ifdef GRAPHICS_DX

#include "Rendering\ShaderProgram.h"

#include <unordered_map>

using std::unordered_map;

class DXShaderProgram : public ShaderProgram
{
public:
    friend class ShaderProgram;

    static DXShaderProgram* Create();

    virtual void Init(string path, ResourceInfo* resourceInfo = NULL);
    virtual void ApplyShader();
    virtual void Delete();
};

#endif