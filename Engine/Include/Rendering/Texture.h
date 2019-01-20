#pragma once

#include "Scene\Resource.h"
#include <string>

class ResourceInfo;

class Texture : public Resource
{
public:
    static Texture* Create();
    static void     Destroy(Texture* texture);
    static Texture* DefaultTexture();

    virtual         ~Texture() {}

    virtual void    Init(std::string filename, ResourceInfo* resourceInfo) = 0;

    virtual void    BindTexture() = 0;
    virtual void    FreeTexture() = 0;

private:
    static Texture* m_defaultTexture;       // TODO this shouldn't live here
};