#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "Color.h"
#include "..\Math\Algebra.h"

class ShaderProgram;

struct Light
{
public: 
    Vector3     position;
    ColorRGB    color;
    GLfloat     power;

    Light();
    Light(Vector3 p, ColorRGB c, GLfloat pow);

    void ApplyLight(ShaderProgram* shader);
};