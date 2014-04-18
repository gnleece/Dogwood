#pragma once

#include "Colour.h"

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

class ShaderProgram;
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

    void SetShader(ShaderProgram* shader);
    void SetTexture(Texture* texture);
    void SetColour(eMatColourType type, ColourRGB colour);

    void ApplyMaterial();

private:
    ShaderProgram*  m_shader;
    Texture*        m_texture;
    ColourRGB       m_colours[NUM_MAT_COLOURS];

    GLint           m_uniColourDiffuse;
    GLint           m_uniColourAmbient;
    GLint           m_uniColourSpecular;
};