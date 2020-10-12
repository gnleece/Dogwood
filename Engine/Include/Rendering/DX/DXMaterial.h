#pragma once

#ifdef GRAPHICS_DX

#include "Rendering\Material.h"

class DXShaderProgram;

class DXMaterial : public Material
{
public:
    friend class Material;

    static DXMaterial* Create();

    virtual void    SetMesh(MeshInstance* mesh);
    virtual void    SetShader(ShaderProgram* shader);
    virtual void    SetColor(string paramName, ColorRGB color);
    virtual void    SetTexture(string paramName, Texture* texture);

    virtual MeshInstance*   GetMesh();
    virtual ShaderProgram*  GetShader();
    virtual ColorRGB        GetColor(string paramName);
    virtual Texture*        GetTexture(string paramName);

    virtual Material*   DeepCopy();

    virtual unordered_map<string, ColorRGB>& GetColors();
    virtual unordered_map<string, Texture*>& GetTextures();
};

#endif