#pragma once

#include "..\Math\Algebra.h"

class ColorRGB
{
public:
    ColorRGB();
    ColorRGB(float red, float green, float blue);
    ColorRGB(Vector3 &v);

    float r;
    float g;
    float b;

    float*  Start();
    Vector3 ToVector();

    static ColorRGB Black;
    static ColorRGB White;
    static ColorRGB Gray;
    static ColorRGB Red;
    static ColorRGB Green;
    static ColorRGB Blue;
    static ColorRGB Yellow;
    static ColorRGB Magenta;
    static ColorRGB Cyan;
};