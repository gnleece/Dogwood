#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "Colour.h"
#include "..\Math\Algebra.h"

class ShaderProgram;

struct Light
{
public: 
    Vector3     position;
    ColourRGB   color;
    GLfloat     power;

    Light();
    Light(Vector3 p, ColourRGB c, GLfloat pow);

    void SetLightForShader(ShaderProgram* shader);
};