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

class RenderManager
{
public:
    static RenderManager& Singleton()
    {
        static RenderManager    singleton;
        return singleton;
    }
    RenderManager() {}

    void Startup(GLFWwindow* gameWindow);
    void Shutdown();

    void SetLight(Light light);
    void SetCamera(Camera camera);

    void RenderScene(GameObject* rootObject);
    void ApplyGlobalParams(ShaderProgram* shader);

private:
    RenderManager(RenderManager const&);
    void operator=(RenderManager const&);

    void SetUniformMatrix(ShaderProgram* shader, ShaderProgram::eShaderParam param, Matrix4x4 & matrix);

    GLFWwindow* m_gameWindow;

    Light       m_light;        // TODO support mutiple light sources
    Camera      m_mainCamera;
    //Camera    m_debugCamera;  // TODO add separate debug camera rendering mode

    Matrix4x4   m_viewMatrix;
    Matrix4x4   m_projMatrix;
};