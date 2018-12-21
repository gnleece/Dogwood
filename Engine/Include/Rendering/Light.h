#pragma once

#include "Color.h"
#include "Math\Algebra.h"

struct Light
{
public:
    Vector3     position;
    ColorRGB    color;
    float       power;

    Light();
    Light(Vector3 p, ColorRGB c, float pow);
};