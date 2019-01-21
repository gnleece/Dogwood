#pragma once

#include "Rendering\Material.h"

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

class ShaderProgramImpl;

class MaterialImpl : public Material
{
public:
    friend class Material;

    static MaterialImpl* Create();

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

    void    ApplyMaterial(Transform& transform, GLint posVBO, GLint normVBO, GLint uvVBO, bool useUVs);
    void    UnapplyMaterial();

private:
    void   SetUniformParam(string paramName, ColorRGB& color);
    void   SetAttribParam(GLint paramID, GLint buffer, int size);
    void   DisableAttribArray(GLint paramID);

    MeshInstance*       m_mesh;
    ShaderProgramImpl*  m_shader;

    unordered_map<string, ColorRGB>  m_colors;
    unordered_map<string, Texture*>  m_textures;

    GLint   m_positionParamID;
    GLint   m_normalParamID;
    GLint   m_uvParamID;
    GLint   m_modelID;
};