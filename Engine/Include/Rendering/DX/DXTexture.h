#pragma once

#ifdef GRAPHICS_DX

#include "Rendering\Texture.h"
#include "Rendering\Image.h"

class DXTexture : public Texture
{
public:
    friend class Texture;

    virtual void    Init(std::string filename, ResourceInfo* resourceInfo);

    virtual void    BindTexture();
    virtual void    FreeTexture();
};

#endif