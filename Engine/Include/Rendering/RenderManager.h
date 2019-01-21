#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "Math\Algebra.h"
#include "Math\Transform.h"
#include "Math\Transformations.h"
#include "Rendering\Camera.h"
#include "Rendering\Light.h"
#include "Rendering\ShaderProgram.h"

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

class DebugDraw;
class GameObjectBase;

class PlatSpecificRenderer
{
public:
    virtual void ApplyLight(Light& light, ShaderProgram* shader) = 0;
};

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

    virtual void SetRootObject(GameObjectBase* rootObject) = 0;
    virtual void SetLight(Light light) = 0;

    virtual Camera& GetCamera() = 0;
    virtual void SetCamera(Camera& camera) = 0;

    virtual void RenderScene() = 0;
    virtual void ApplyGlobalParams(ShaderProgram* shader) = 0;

    virtual int GetViewportWidth() = 0;
    virtual int GetViewportHeight() = 0;

    virtual bool SettingsDirty() = 0;

    virtual DebugDraw* GetDebugDraw() = 0;

private:
    static RenderManager* Create();
    static void Destroy(RenderManager* renderManager);
};