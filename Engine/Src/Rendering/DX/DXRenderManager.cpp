#ifdef GRAPHICS_DX

#include "Rendering\DX\DXDebugDraw.h"
#include "Rendering\DX\DXRenderManager.h"
#include "Rendering\DX\DXShaderProgram.h"

RenderManager* RenderManager::Create()
{
    return new DXRenderManager();
}

void RenderManager::Destroy(RenderManager* renderManager)
{
    delete renderManager;
}

void DXRenderManager::Startup(int viewportWidth, int viewportHeight)
{
   
}

void DXRenderManager::Shutdown()
{

}

Light& DXRenderManager::GetLight()
{
    return Light();
}

void DXRenderManager::SetLight(Light& light)
{

}

Camera& DXRenderManager::GetCamera()
{
    return Camera();
}

void DXRenderManager::SetCamera(Camera& camera)
{

}

void DXRenderManager::RenderScene(Scene* scene)
{

}

int DXRenderManager::GetViewportWidth()
{
    return 0;
}

int DXRenderManager::GetViewportHeight()
{
    return 0;
}

DebugDraw* DXRenderManager::GetDebugDraw()
{
    return NULL;
}

#endif