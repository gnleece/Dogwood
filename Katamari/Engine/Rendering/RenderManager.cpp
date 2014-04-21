#include "RenderManager.h"

#include "..\GameObject.h"

void RenderManager::Startup(GLFWwindow* gameWindow)
{
    m_gameWindow = gameWindow;

    // Prepare projection matrix
    // TODO pass params in properly
    m_projMatrix = PerspectiveProjection(45.0f, (float)1024 / 768, 0.1f, 1000.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void RenderManager::Shutdown()
{
}

void RenderManager::SetLight(Light light)
{
    m_light = light;
}

void RenderManager::SetCamera(Camera camera)
{
    m_mainCamera = camera;
    m_viewMatrix = LookAt(m_mainCamera);
}

void RenderManager::RenderScene(GameObject* rootObject)
{
    // Clear the screen to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render game objects
    rootObject->Render(Matrix4x4::Identity, false);

    // Swap buffers
    glfwSwapBuffers(m_gameWindow);
}

void RenderManager::ApplyGlobalParams(ShaderProgram* shader)
{
    m_light.ApplyLight(shader);

    SetUniformMatrix(shader, ShaderProgram::UNI_VIEW, m_viewMatrix);
    SetUniformMatrix(shader, ShaderProgram::UNI_PROJ, m_projMatrix);
}

void RenderManager::SetUniformMatrix(ShaderProgram* shader, ShaderProgram::eShaderParam param, Matrix4x4 & matrix)
{
    GLint paramLocation = shader->GetParamLocation(param);
    glUniformMatrix4fv(paramLocation, 1, GL_FALSE, matrix.Transpose().Start());
}