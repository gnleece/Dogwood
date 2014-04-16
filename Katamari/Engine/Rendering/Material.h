#pragma once

#include "Colour.h"

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

    void SetTexture(Texture* texture);
    void SetColour(eMatColourType type, ColourRGB colour);

    void ApplyMaterial(GLint uniColour);

private:
    Texture*    m_texture;
    ColourRGB   m_colours[NUM_MAT_COLOURS];
};