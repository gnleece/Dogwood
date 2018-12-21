#include "Rendering\Light.h"

Light::Light()
{
    position = Vector3(0.0f, 0.0f, 0.0f);
    color = ColorRGB(1.0f, 1.0f, 1.0f);
    power = 15.0f;
}

Light::Light(Vector3 p, ColorRGB c, float pow)
{
    position = p;
    color = c;
    power = pow;
}