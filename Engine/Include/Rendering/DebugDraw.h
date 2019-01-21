#pragma once

#include "Math\Algebra.h"
#include "Math\Transform.h"
#include "Rendering\Color.h"

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

class Material;

class DebugDraw
{
public:
    const static int MAX_LINES_PER_FRAME = 20;  // TODO this is pretty lame. resize dynamically

    static DebugDraw* Create();
    static void Destroy(DebugDraw* debugDraw);

    virtual void DrawLine(Vector3& a, Vector3& b, ColorRGB& Color) = 0;
    virtual void RenderLines() = 0;

    virtual void DrawSphere(Matrix4x4& transform, ColorRGB color, bool useDepth = true) = 0;
    virtual void DrawCube(Matrix4x4& transform, ColorRGB color, bool useDepth = true) = 0;
    virtual void DrawCapsule(Matrix4x4& transform, ColorRGB color, bool useDepth = true) = 0;
};