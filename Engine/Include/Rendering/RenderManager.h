#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "..\Math\Algebra.h"
#include "..\Math\Transform.h"
#include "..\Math\Transformations.h"
#include "Light.h"
#include "ShaderProgram.h"

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

class GameObject;

struct RenderConfig
{
public:
    int width = 640;
    int height = 480;
    float FOV = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;
    ColourRGB clearColour = ColourRGB::Black;
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
        SHADER_GOURAUD,
        NUM_COMMON_SHADERS
    };

    void            Startup(RenderConfig& config);
    void            Shutdown();

    void            SetRootObject(GameObject* rootObject);
    void            SetLight(Light light);
    void            SetCamera(Camera camera);
    void            SetView(Matrix4x4& view);
    void            SetClearColour(ColourRGB colour);

    Transform&      GetView();
    Transform&      GetProjection();
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

    GameObject*     m_rootObject;
    Light           m_light;        // TODO support multiple light sources
    ColourRGB       m_clearColour;

    Transform       m_viewMatrix;
    Transform       m_projMatrix;

    bool            m_dirty;

    ShaderProgram*  m_commonShaders[NUM_COMMON_SHADERS];
};