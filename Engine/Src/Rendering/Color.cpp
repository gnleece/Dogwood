#include "Rendering\Color.h"

ColorRGB ColorRGB::Black      = ColorRGB(0.0f, 0.0f, 0.0f);
ColorRGB ColorRGB::White      = ColorRGB(1.0f, 1.0f, 1.0f);
ColorRGB ColorRGB::Gray       = ColorRGB(0.5f, 0.5f, 0.5f);
ColorRGB ColorRGB::Red        = ColorRGB(1.0f, 0.0f, 0.0f);
ColorRGB ColorRGB::Green      = ColorRGB(0.0f, 1.0f, 0.0f);
ColorRGB ColorRGB::Blue       = ColorRGB(0.0f, 0.0f, 1.0f);
ColorRGB ColorRGB::Yellow     = ColorRGB(1.0f, 1.0f, 0.0f);
ColorRGB ColorRGB::Magenta    = ColorRGB(1.0f, 0.0f, 1.0f);
ColorRGB ColorRGB::Cyan       = ColorRGB(0.0f, 1.0f, 1.0f);

ColorRGB::ColorRGB()
{
    r = 0.0f;
    g = 0.0f;
    b = 0.0f;
}

ColorRGB::ColorRGB(float red, float green, float blue)
{
    r = red;
    g = green;
    b = blue;
}

ColorRGB::ColorRGB(Vector3 &v)
{
    r = v[0];
    g = v[1];
    b = v[2];
}

float* ColorRGB::Start()
{
    return &r;
}

Vector3 ColorRGB::ToVector()
{
    return Vector3(r, g, b);
}