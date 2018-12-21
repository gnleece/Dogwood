#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "Math\Algebra.h"
#include "Math\Transform.h"
#include "Math\Transformations.h"
#include "Camera.h"
#include "Light.h"
#include "ShaderProgram.h"

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

class GameObjectBase;

class PlatSpecificRenderer
{
public:
    virtual void ApplyLight(Light& light, ShaderProgram* shader) = 0;
};

class RenderManager
{
public:
    static RenderManager& Singleton()
    {
        static RenderManager    singleton;
        return singleton;
    }
    RenderManager() {}

    enum eCommonShader
    {
        SHADER_UNLIT,
        SHADER_UNLIT_UNI_COLOR,
        SHADER_UNLIT_TEXTURE,
        SHADER_GOURAUD,
        NUM_COMMON_SHADERS
    };

    void            Startup(int viewportWidth, int viewportHeight);
    void            Shutdown();

    void            SetRootObject(GameObjectBase* rootObject);
    void            SetLight(Light light);

    Camera&         GetCamera();
    void            SetCamera(Camera& camera);

    void            RenderScene();
    void            ApplyGlobalParams(ShaderProgram* shader);

    int             GetViewportWidth();
    int             GetViewportHeight();

    bool            SettingsDirty();

    ShaderProgram*  GetCommonShader(eCommonShader name);

private:
    RenderManager(RenderManager const&);

    void            LoadCommonShaders();

    PlatSpecificRenderer* m_platSpecificRenderer;

    int             m_viewportWidth;
    int             m_viewportHeight;
    Camera          m_camera;                   // TODO support multiple cameras
    Light           m_light;                    // TODO support multiple light sources
    GameObjectBase* m_rootObject;

    bool            m_dirty;

    ShaderProgram*  m_commonShaders[NUM_COMMON_SHADERS];
};