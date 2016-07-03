#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "Math\Algebra.h"
#include "Math\Transform.h"
#include "Math\Transformations.h"
#include "Light.h"
#include "ShaderProgram.h"

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

class GameObjectBase;

struct RenderConfig
{
public:
    int width = 640;
    int height = 480;
    float FOV = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;
    ColorRGB clearColor = ColorRGB::Black;
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

    void            Startup(RenderConfig& config);
    void            Shutdown();

    void            SetRootObject(GameObjectBase* rootObject);
    void            SetLight(Light light);
    void            SetClearColor(ColorRGB Color);

    void            SetCamera(Camera camera);
    void            SetCameraTransform(Transform& transform);
    void            SetCameraTransform(Matrix4x4& worldMatrix);
    Transform&      GetCameraTransform();

    void            SetViewTransform(Transform& transform);
    Transform&      GetViewTransform();
    Transform&      GetProjectionTransform();
    RenderConfig&   GetConfig();

    void            RenderScene();
    void            ApplyGlobalParams(ShaderProgram* shader);

    bool            SettingsDirty();

    ShaderProgram*  GetCommonShader(eCommonShader name);

    Vector2         ToScreenSpace(Vector3 worldPosition);

private:
    RenderManager(RenderManager const&);
    void operator=(RenderManager const&);

    void            LoadCommonShaders();

    RenderConfig    m_config;

    GameObjectBase* m_rootObject;
    Light           m_light;        // TODO support multiple light sources
    ColorRGB        m_clearColor;

    Transform       m_viewTransform;
    Transform       m_cameraTransform;          // Always the inverse of m_viewTransform
    Transform       m_projectionTransform;

    bool            m_dirty;

    ShaderProgram*  m_commonShaders[NUM_COMMON_SHADERS];
};