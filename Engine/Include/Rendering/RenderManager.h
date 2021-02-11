#pragma once

#include "Math\Algebra.h"
#include "Math\Transform.h"
#include "Math\Transformations.h"
#include "Rendering\Camera.h"
#include "Rendering\Light.h"
#include "Rendering\ShaderProgram.h"

class DebugDraw;
class GameObjectBase;
class Scene;

class RenderManager
{
public:
    static RenderManager* Singleton()
    {
        static RenderManager* singleton = RenderManager::Create();
        return singleton;
    }
    RenderManager() {}

    RenderManager(RenderManager const&) = delete;
    void operator=(RenderManager const&) = delete;

    enum eCommonShader
    {
        SHADER_UNLIT,
        SHADER_UNLIT_UNI_COLOR,
        SHADER_UNLIT_TEXTURE,
        SHADER_GOURAUD,
        NUM_COMMON_SHADERS
    };

    virtual void Startup(int viewportWidth, int viewportHeight) = 0;
    virtual void Shutdown() = 0;

    virtual Light& GetLight() = 0;
    virtual void SetLight(Light& light) = 0;

    virtual Camera& GetCamera() = 0;
    virtual void SetCamera(Camera& camera) = 0;

    virtual void RenderScene(Scene* scene) = 0;

    virtual int GetViewportWidth() = 0;
    virtual int GetViewportHeight() = 0;

    virtual DebugDraw* GetDebugDraw() = 0;

private:
    static RenderManager* Create();
    static void Destroy(RenderManager* renderManager);
};