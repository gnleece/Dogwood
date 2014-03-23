#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "Colour.h"
#include "..\Math\Algebra.h"

struct Light
{
public: 
    Vector3 position;
    ColourRGB color;
    GLfloat power;

    Light() : position(Vector3(0.0f, 0.0f, 0.0f)), color(ColourRGB(1.0f, 1.0f, 1.0f)), power(15.0f) {}
    Light(Vector3 p, ColourRGB c, GLfloat pow) : position(p), color(c), power(pow) {}

    void SetLightForShader(GLuint shaderID);
};