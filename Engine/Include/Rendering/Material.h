#pragma once

#include "..\Math\Transform.h"
#include "Colour.h"
#include "ShaderProgram.h"

#include <unordered_map>

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

using std::unordered_map;
using std::vector;

class Texture;

class Material
{
public:
    Material();

    void            SetShader(ShaderProgram* shader);
    void            SetTexture(Texture* texture);
    void            SetColor(GLint paramID, ColourRGB color);
    void            SetColor(string paramName, ColourRGB color);

    ShaderProgram*  GetShader();
    Texture*        GetTexture();
    ColourRGB       GetColor(GLint paramID);
    ColourRGB       GetColor(string paramName);
    unordered_map<GLint, ColourRGB>& GetColorList();

    void            ApplyMaterial(GLint posVBO, GLint normVBO, GLint uvVBO, Transform& transform);
    void            UnapplyMaterial();

    Material*       DeepCopy();

private:
    void            SetUniformParam(GLint paramID, ColourRGB& color);
    void            SetAttribParam(GLint paramID, GLint buffer, int size);
    void            DisableAttribArray(GLint paramID);

    ShaderProgram*                  m_shader;
    Texture*                        m_texture;

    GLint                           m_positionParamID;
    GLint                           m_normalParamID;
    GLint                           m_uvParamID;
    GLint                           m_modelID;

    unordered_map<GLint, ColourRGB> m_uniformColors;
};