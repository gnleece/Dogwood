#pragma once

#include "..\Math\Algebra.h"

class ColourRGB
{
public:
    ColourRGB()                                      { r = 0.0f; g = 0.0f;  b = 0.0f; }
    ColourRGB(float red, float green, float blue)    { r = red;  g = green; b = blue; }
    ColourRGB(Vector3 &v)                            { r = v[0]; g = v[1];  b = v[2]; }

    float r;
    float g;
    float b;

    float* Start() { return &r; }

    static ColourRGB Black;
    static ColourRGB White;
    static ColourRGB Gray;
    static ColourRGB Red;
    static ColourRGB Green;
    static ColourRGB Blue;
    static ColourRGB Yellow;
    static ColourRGB Magenta;
    static ColourRGB Cyan;
};