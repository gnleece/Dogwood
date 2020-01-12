#include "Rendering\GL\GLDebugDraw.h"
#include "Rendering\GL\GLRenderManager.h"
#include "Rendering\GL\GLShaderProgram.h"

#include "GameObject.h"
#include "Physics\CollisionEngine.h"

#include "Rendering\GL\GLRenderer.h"			// TODO [GL+DX] ifdef

RenderManager* RenderManager::Create()
{
    return new GLRenderManager();
}

void RenderManager::Destroy(RenderManager* renderManager)
{
    delete renderManager;
}

void GLRenderManager::Startup(int viewportWidth, int viewportHeight)
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
    m_debugDraw = (GLDebugDraw*)DebugDraw::Create();
    m_debugDraw->Startup(this);
}

void GLRenderManager::Shutdown()
{
    m_debugDraw->Shutdown();
    DebugDraw::Destroy(m_debugDraw);
    delete m_platSpecificRenderer;
}

void GLRenderManager::SetRootObject(GameObjectBase* rootObject)
{
    m_rootObject = rootObject;
    m_dirty = true;
}

void GLRenderManager::SetLight(Light light)
{
    m_light = light;
    m_dirty = true;
}

Camera& GLRenderManager::GetCamera()
{
    return m_camera;
}

void GLRenderManager::SetCamera(Camera& camera)
{
    m_camera = camera;
    m_camera.SetPixelWidth(m_viewportWidth);
    m_camera.SetPixelHeight(m_viewportHeight);
}

void GLRenderManager::RenderScene()
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

void GLRenderManager::ApplyGlobalParams(ShaderProgram* shader)
{
    GLShaderProgram* shaderImpl = (GLShaderProgram*)shader;
    if (shaderImpl != NULL)
    {
        // Light
        m_platSpecificRenderer->ApplyLight(m_light, shader);

        Vector3 position = m_camera.GetPosition();
        Vector3 direction = m_camera.GetDirection();
        Vector3 up = m_camera.GetUp();

        // View matrix
        GLint viewLocation = shaderImpl->GetUniformLocation("view");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, m_camera.GetViewTransform().GetLocalMatrix().Transpose().Start());

        // Projection matrix
        GLint projLocation = shaderImpl->GetUniformLocation("proj");
        glUniformMatrix4fv(projLocation, 1, GL_FALSE, m_camera.GetProjectionTransform().GetLocalMatrix().Transpose().Start());

        m_dirty = false;
        m_camera.ClearDirtyFlag();
    }
}

int GLRenderManager::GetViewportWidth()
{
    return m_viewportWidth;
}

int GLRenderManager::GetViewportHeight()
{
    return m_viewportHeight;
}

bool GLRenderManager::SettingsDirty()
{
    return m_dirty || m_camera.IsDirty();
}

DebugDraw* GLRenderManager::GetDebugDraw()
{
    return m_debugDraw;
}

GLShaderProgram* GLRenderManager::GetCommonShader(eCommonShader name)
{
    if (name >= 0 && name < NUM_COMMON_SHADERS)
    {
        return m_commonShaders[name];
    }
    return NULL;
}

void GLRenderManager::LoadCommonShaders()
{
    // TODO this is pretty awful
    m_commonShaders[SHADER_UNLIT] = GLShaderProgram::Create();
    (m_commonShaders[SHADER_UNLIT])->Init("..\\Engine\\Assets\\Shaders\\Unlit.glsl");

    m_commonShaders[SHADER_UNLIT_UNI_COLOR] = GLShaderProgram::Create();
    (m_commonShaders[SHADER_UNLIT_UNI_COLOR])->Init("..\\Engine\\Assets\\Shaders\\UnlitUniformColor.glsl");

    m_commonShaders[SHADER_UNLIT_TEXTURE] = GLShaderProgram::Create();
    (m_commonShaders[SHADER_UNLIT_TEXTURE])->Init("..\\Engine\\Assets\\Shaders\\UnlitTexture.glsl");

    m_commonShaders[SHADER_GOURAUD] = GLShaderProgram::Create();
    (m_commonShaders[SHADER_GOURAUD])->Init("..\\Engine\\Assets\\Shaders\\Gouraud.glsl");
}