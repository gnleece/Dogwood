#include "Rendering\RenderManager.h"

#include "GameObject.h"
#include "Debugging\DebugDraw.h"

void RenderManager::Startup(RenderConfig& config)
{
    m_dirty = true;
    m_rootObject = NULL;

    // Prepare projection matrix
    float aspect = (float)config.width / config.height;
    m_projMatrix = PerspectiveProjection(config.FOV, aspect, config.nearPlane, config.farPlane);
    glViewport(0, 0, config.width, config.height);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    glewExperimental = GL_TRUE;
    glewInit();

    //DebugDraw::Singleton().Startup();     // TODO fixme qt-opengl-convert
}

void RenderManager::Shutdown()
{
    //DebugDraw::Singleton().Shutdown();
}

void RenderManager::SetRootObject(GameObject* rootObject)
{
    m_rootObject = rootObject;
    m_dirty = true;
}

void RenderManager::SetLight(Light light)
{
    m_light = light;
    m_dirty = true;
}

void RenderManager::SetCamera(Camera camera)
{
    m_viewMatrix = LookAt(camera);
    m_dirty = true;
}

void RenderManager::SetView(Matrix4x4& view)
{
    m_viewMatrix = view;
    m_dirty = true;
}

Matrix4x4& RenderManager::GetView()
{
    return m_viewMatrix;
}

void RenderManager::RenderScene()
{
    // Clear the screen to black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render game objects
    if (m_rootObject != NULL)
    {
        m_rootObject->Render(Transform::Identity, false);
    }

    // Draw debug gnomon. TODO remove me
    //DebugDraw::Singleton().DrawLine(Vector3(-1.5f, -1.f, -3.f), Vector3(-0.5f, -1.f, -3.f), ColourRGB::Red);
    //DebugDraw::Singleton().DrawLine(Vector3(-1.5f, -1.f, -3.f), Vector3(-1.5f, 0.f, -3.f), ColourRGB::Green);
    //DebugDraw::Singleton().DrawLine(Vector3(-1.5f, -1.f, -3.f), Vector3(-1.5f, -1.f, -4.f), ColourRGB::Blue);

    //DebugDraw::Singleton().RenderLines();

    // Swap buffers
    //m_gameWindow->SwapBuffers();  // TODO fixme qt-opengl-convert
}

void RenderManager::ApplyGlobalParams(ShaderProgram* shader)
{
    m_light.ApplyLight(shader);

    SetUniformMatrix(shader, ShaderProgram::UNI_VIEW, m_viewMatrix);
    SetUniformMatrix(shader, ShaderProgram::UNI_PROJ, m_projMatrix);

    m_dirty = false;
}

bool RenderManager::SettingsDirty()
{
    return m_dirty;
}

void RenderManager::SetUniformMatrix(ShaderProgram* shader, ShaderProgram::eShaderParam param, Matrix4x4 & matrix)
{
    GLint paramLocation = shader->GetParamLocation(param);
    glUniformMatrix4fv(paramLocation, 1, GL_FALSE, matrix.Transpose().Start());
}