#ifdef GRAPHICS_DX

#include "Rendering/DX/DXDebugDraw.h"

#include "Rendering/Material.h"
#include "Rendering/RenderManager.h"

DebugDraw* DebugDraw::Create()
{
    return new DXDebugDraw();
}

void DebugDraw::Destroy(DebugDraw* debugDraw)
{
    delete debugDraw;
}

void DXDebugDraw::Startup(DXRenderManager* renderManager)
{

}

void DXDebugDraw::Shutdown()
{

}

void DXDebugDraw::DrawLine(Vector3& a, Vector3& b, ColorRGB& Color)
{

}

void DXDebugDraw::RenderLines()
{
   
}

void DXDebugDraw::DrawSphere(Matrix4x4& transform, ColorRGB color, bool useDepth)
{

}

void DXDebugDraw::DrawCube(Matrix4x4& transform, ColorRGB color, bool useDepth)
{

}

void DXDebugDraw::DrawCapsule(Matrix4x4& transform, ColorRGB color, bool useDepth)
{

}

#endif