#pragma once

class ColourRGB
{
public:
    ColourRGB()                                      { r = 0.0f; g = 0.0f;  b = 0.0f; }
    ColourRGB(float red, float green, float blue)    { r = red;  g = green; b = blue; }

    float r;
    float g;
    float b;

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