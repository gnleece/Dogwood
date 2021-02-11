#pragma once

#ifdef GRAPHICS_DX

#include "Rendering/RenderManager.h"

class DebugDraw;

class DXRenderManager : public RenderManager
{
public:
    friend class RenderManager;
    friend class DebugDraw;

    virtual void Startup(int viewportWidth, int viewportHeight);
    virtual void Shutdown();

    virtual Light& GetLight();
    virtual void SetLight(Light& light);

    virtual Camera& GetCamera();
    virtual void SetCamera(Camera& camera);

    virtual void RenderScene(Scene* scene);

    virtual int GetViewportWidth();
    virtual int GetViewportHeight();

    virtual DebugDraw* GetDebugDraw();
};

#endif