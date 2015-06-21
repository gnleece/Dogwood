#pragma once

#include "..\Math\Algebra.h"

class ColourRGB
{
public:
    ColourRGB();
    ColourRGB(float red, float green, float blue);
    ColourRGB(Vector3 &v);

    float r;
    float g;
    float b;

    float*  Start();
    Vector3 ToVector();

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