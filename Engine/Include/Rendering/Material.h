#pragma once

#include "..\Math\Transform.h"
#include "Colour.h"
#include "ShaderProgram.h"

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

class Texture;

class Material
{
public:
    enum eMatColourType 
    { 
        MAT_COLOUR_DIFFUSE,
        MAT_COLOUR_AMBIENT,
        MAT_COLOUR_SPECULAR,
        NUM_MAT_COLOURS
    };

    Material();

    void            SetShader(ShaderProgram* shader);
    void            SetTexture(Texture* texture);
    void            SetColour(eMatColourType type, ColourRGB colour);

    ShaderProgram*  GetShader();
    Texture*        GetTexture();
    ColourRGB       GetColour(eMatColourType type);

    void            ApplyMaterial(GLint posVBO, GLint normVBO, GLint uvVBO, Transform& transform);
    void            UnapplyMaterial();

    Material*       DeepCopy();

private:
    void            SetUniformParam(ShaderProgram::eShaderParam param, eMatColourType colour);
    void            SetAttribParam(ShaderProgram::eShaderParam param, GLint buffer, int size);
    void            DisableAttribArray(ShaderProgram::eShaderParam param);

    ShaderProgram*  m_shader;
    Texture*        m_texture;
    ColourRGB       m_colours[NUM_MAT_COLOURS];
};