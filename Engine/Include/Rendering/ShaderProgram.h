#pragma once

#include "Scene\Resource.h"

#include <string>

class ResourceInfo;

using std::string;

class ShaderProgram : public Resource
{
public:
    static ShaderProgram* Create();
    static void Destroy(ShaderProgram* shader);

    virtual void Init(string path, ResourceInfo* resourceInfo = NULL) = 0;
    virtual void ApplyShader() = 0;
    virtual void Delete() = 0;
};