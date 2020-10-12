#ifdef GRAPHICS_DX

#include "Rendering/Material.h"
#include "Rendering/DX/DXMaterial.h"

#include "Rendering/RenderManager.h"
#include "Rendering/ShaderProgram.h"
#include "Rendering/Texture.h"

Material* Material::Create()
{
    return DXMaterial::Create();
}

void Material::Destroy(Material* material)
{
    delete material;
}

DXMaterial* DXMaterial::Create()
{
    return new DXMaterial();
}

void DXMaterial::SetMesh(MeshInstance* mesh)
{

}

void DXMaterial::SetShader(ShaderProgram* shader)
{
    
}

void DXMaterial::SetColor(string paramName, ColorRGB color)
{

}

void DXMaterial::SetTexture(string paramName, Texture* texture)
{

}

MeshInstance* DXMaterial::GetMesh()
{
    return NULL;
}

ShaderProgram* DXMaterial::GetShader()
{
    return NULL;
}

ColorRGB DXMaterial::GetColor(string paramName)
{
    return ColorRGB::Black;
}

Texture* DXMaterial::GetTexture(string paramName)
{
    return NULL;
}

unordered_map<string, ColorRGB>& DXMaterial::GetColors()
{
    return unordered_map<string, ColorRGB>();
}

unordered_map<string, Texture*>& DXMaterial::GetTextures()
{
    return unordered_map<string, Texture*>();
}

Material* DXMaterial::DeepCopy()
{
    return NULL;
}

#endif