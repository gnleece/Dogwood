#include "Rendering\RenderManager.h"

#include "GameObject.h"
#include "Debugging\DebugDraw.h"
#include "Physics\CollisionEngine.h"

#include "Rendering/OpenGL/GLRenderer.h"			// TODO [GL+DX] ifdef

void RenderManager::Startup(int viewportWidth, int viewportHeight)
{
    m_dirty = true;
    m_rootObject = NULL;
    m_viewportWidth = viewportWidth;
    m_viewportHeight = viewportHeight;

    m_platSpecificRenderer = new GLRenderer();		// TODO [GL+DX] ifdef

    // OpenGL setup
    glViewport(0, 0, m_viewportWidth, m_viewportHeight);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    glewExperimental = GL_TRUE;
    GLenum ret = glewInit();
    if (ret != 0)
    {
        printf("Error in glewInit! Abort.\n");
    }

    // Common asset setup
    LoadCommonShaders();

    // Dependent manager setup
    DebugDraw::Singleton().Startup();
}

void RenderManager::Shutdown()
{
    DebugDraw::Singleton().Shutdown();
    delete m_platSpecificRenderer;
}

void RenderManager::SetRootObject(GameObjectBase* rootObject)
{
    m_rootObject = rootObject;
    m_dirty = true;
}

void RenderManager::SetLight(Light light)
{
    m_light = light;
    m_dirty = true;
}

Camera& RenderManager::GetCamera()
{
    return m_camera;
}

void RenderManager::SetCamera(Camera& camera)
{
    m_camera = camera;
    m_camera.SetPixelWidth(m_viewportWidth);
    m_camera.SetPixelHeight(m_viewportHeight);
}

void RenderManager::RenderScene()
{
    // Clear the screen to black
    ColorRGB clearColor = m_camera.GetClearColor();
    glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render game objects
    if (m_rootObject != NULL)
    {
        m_rootObject->Render(false);
    }

    // Render debug info    // TODO this is hacky, RenderManager shouldn't have reference to CollisionEngine
    CollisionEngine::Singleton().DrawDebugInfo();
}

void RenderManager::ApplyGlobalParams(ShaderProgram* shader)
{
    // Light
    m_platSpecificRenderer->ApplyLight(m_light, shader);

    Vector3 position = m_camera.GetPosition();
    Vector3 direction = m_camera.GetDirection();
    Vector3 up = m_camera.GetUp();

    // View matrix
    GLint viewLocation = shader->GetUniformLocation("view");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, m_camera.GetViewTransform().GetLocalMatrix().Transpose().Start());

    // Projection matrix
    GLint projLocation = shader->GetUniformLocation("proj");
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, m_camera.GetProjectionTransform().GetLocalMatrix().Transpose().Start());

    m_dirty = false;
    m_camera.ClearDirtyFlag();
}

int RenderManager::GetViewportWidth()
{
    return m_viewportWidth;
}

int RenderManager::GetViewportHeight()
{
    return m_viewportHeight;
}

bool RenderManager::SettingsDirty()
{
    return m_dirty || m_camera.IsDirty();
}

ShaderProgram* RenderManager::GetCommonShader(eCommonShader name)
{
    if (name >= 0 && name < NUM_COMMON_SHADERS)
        return m_commonShaders[name];
    return NULL;
}

void RenderManager::LoadCommonShaders()
{
    // TODO this is pretty awful
    m_commonShaders[SHADER_UNLIT] = new ShaderProgram();
    (m_commonShaders[SHADER_UNLIT])->Load("..\\Engine\\Assets\\Shaders\\Unlit.glsl");

    m_commonShaders[SHADER_UNLIT_UNI_COLOR] = new ShaderProgram();
    (m_commonShaders[SHADER_UNLIT_UNI_COLOR])->Load("..\\Engine\\Assets\\Shaders\\UnlitUniformColor.glsl");

    m_commonShaders[SHADER_UNLIT_TEXTURE] = new ShaderProgram();
    (m_commonShaders[SHADER_UNLIT_TEXTURE])->Load("..\\Engine\\Assets\\Shaders\\UnlitTexture.glsl");

    m_commonShaders[SHADER_GOURAUD] = new ShaderProgram();
    (m_commonShaders[SHADER_GOURAUD])->Load("..\\Engine\\Assets\\Shaders\\Gouraud.glsl");
}