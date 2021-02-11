#ifdef GRAPHICS_DX

#include "Rendering\DX\DXTexture.h"
#include "Rendering\Image.h"

#include <fstream>

Texture* Texture::Create()
{
    return new DXTexture();
}

void Texture::Destroy(Texture* texture)
{
    delete texture;
}

Texture* Texture::DefaultTexture()
{
    return m_defaultTexture;
}

Texture* Texture::m_defaultTexture = NULL;

void DXTexture::Init(std::string filename, ResourceInfo* resourceInfo)
{
    
}

void DXTexture::BindTexture()
{

}

void DXTexture::FreeTexture()
{

}

#endif
