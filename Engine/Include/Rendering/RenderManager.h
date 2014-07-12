#pragma once

#include "..\Math\Algebra.h"
#include "..\Math\Transformations.h"
#include "Light.h"
#include "ShaderProgram.h"

#define GLEW_STATIC
#include <GL/glew.h>

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

    void        Startup(RenderConfig& config);
    void        Shutdown();

    void        SetLight(Light light);

    void        SetCamera(Camera camera);
    void        SetView(Matrix4x4& view);
    Matrix4x4&  GetView();

    void        RenderScene(GameObject* rootObject);
    void        ApplyGlobalParams(ShaderProgram* shader);

    bool        SettingsDirty();

private:
    RenderManager(RenderManager const&);
    void operator=(RenderManager const&);

    void        SetUniformMatrix(ShaderProgram* shader, ShaderProgram::eShaderParam param, Matrix4x4& matrix);

    Light       m_light;        // TODO support mutiple light sources

    Matrix4x4   m_viewMatrix;
    Matrix4x4   m_projMatrix;

    bool        m_dirty;
};