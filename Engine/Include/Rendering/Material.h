#pragma once

#include "Math\Transform.h"
#include "Color.h"

#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;
using std::vector;

class MeshInstance;
class ShaderProgram;
class Texture;

class Material
{
public:
    static Material* Create();
    static void Destroy(Material* material);

    virtual ~Material() {}

    virtual void    SetMesh(MeshInstance* mesh) = 0;
    virtual void    SetShader(ShaderProgram* shader) = 0;
    virtual void    SetColor(string paramName, ColorRGB color) = 0;
    virtual void    SetTexture(string paramName, Texture* texture) = 0;

    virtual MeshInstance*   GetMesh() = 0;
    virtual ShaderProgram*  GetShader() = 0;
    virtual ColorRGB        GetColor(string paramName) = 0;
    virtual Texture*        GetTexture(string paramName) = 0;

    virtual Material*       DeepCopy() = 0;

    virtual unordered_map<string, ColorRGB>& GetColors() = 0;
    virtual unordered_map<string, Texture*>& GetTextures() = 0;
};