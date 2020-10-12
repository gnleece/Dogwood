#pragma once

#ifdef GRAPHICS_DX

#include "Rendering/DebugDraw.h"

class DXRenderManager;

class DXDebugDraw : public DebugDraw
{
public:
    friend class DebugDraw;
    friend class RenderManager;

    virtual void DrawLine(Vector3& a, Vector3& b, ColorRGB& Color);
    virtual void RenderLines();

    virtual void DrawSphere(Matrix4x4& transform, ColorRGB color, bool useDepth = true);
    virtual void DrawCube(Matrix4x4& transform, ColorRGB color, bool useDepth = true);
    virtual void DrawCapsule(Matrix4x4& transform, ColorRGB color, bool useDepth = true);

    void Startup(DXRenderManager* renderManager);
    void Shutdown();
};

#endif