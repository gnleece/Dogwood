#include "Rendering/OpenGL/DebugDrawImpl.h"
#include "Rendering/OpenGL/RenderManagerImpl.h"
#include "Rendering/OpenGL/ShaderProgramImpl.h"

#include "GameObject.h"
#include "Physics\CollisionEngine.h"

#include "Rendering/OpenGL/GLRenderer.h"			// TODO [GL+DX] ifdef

RenderManager* RenderManager::Create()
{
    return new RenderManagerImpl();
}

void RenderManager::Destroy(RenderManager* renderManager)
{
    delete renderManager;
}

void RenderManagerImpl::Startup(int viewportWidth, int viewportHeight)
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
    m_debugDraw = (DebugDrawImpl*)DebugDraw::Create();
    m_debugDraw->Startup(this);
}

void RenderManagerImpl::Shutdown()
{
    m_debugDraw->Shutdown();
    DebugDraw::Destroy(m_debugDraw);
    delete m_platSpecificRenderer;
}

void RenderManagerImpl::SetRootObject(GameObjectBase* rootObject)
{
    m_rootObject = rootObject;
    m_dirty = true;
}

void RenderManagerImpl::SetLight(Light light)
{
    m_light = light;
    m_dirty = true;
}

Camera& RenderManagerImpl::GetCamera()
{
    return m_camera;
}

void RenderManagerImpl::SetCamera(Camera& camera)
{
    m_camera = camera;
    m_camera.SetPixelWidth(m_viewportWidth);
    m_camera.SetPixelHeight(m_viewportHeight);
}

void RenderManagerImpl::RenderScene()
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

void RenderManagerImpl::ApplyGlobalParams(ShaderProgram* shader)
{
    ShaderProgramImpl* shaderImpl = (ShaderProgramImpl*)shader;
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

int RenderManagerImpl::GetViewportWidth()
{
    return m_viewportWidth;
}

int RenderManagerImpl::GetViewportHeight()
{
    return m_viewportHeight;
}

bool RenderManagerImpl::SettingsDirty()
{
    return m_dirty || m_camera.IsDirty();
}

DebugDraw* RenderManagerImpl::GetDebugDraw()
{
    return m_debugDraw;
}

ShaderProgramImpl* RenderManagerImpl::GetCommonShader(eCommonShader name)
{
    if (name >= 0 && name < NUM_COMMON_SHADERS)
    {
        return m_commonShaders[name];
    }
    return NULL;
}

void RenderManagerImpl::LoadCommonShaders()
{
    // TODO this is pretty awful
    m_commonShaders[SHADER_UNLIT] = ShaderProgramImpl::Create();
    (m_commonShaders[SHADER_UNLIT])->Init("..\\Engine\\Assets\\Shaders\\Unlit.glsl");

    m_commonShaders[SHADER_UNLIT_UNI_COLOR] = ShaderProgramImpl::Create();
    (m_commonShaders[SHADER_UNLIT_UNI_COLOR])->Init("..\\Engine\\Assets\\Shaders\\UnlitUniformColor.glsl");

    m_commonShaders[SHADER_UNLIT_TEXTURE] = ShaderProgramImpl::Create();
    (m_commonShaders[SHADER_UNLIT_TEXTURE])->Init("..\\Engine\\Assets\\Shaders\\UnlitTexture.glsl");

    m_commonShaders[SHADER_GOURAUD] = ShaderProgramImpl::Create();
    (m_commonShaders[SHADER_GOURAUD])->Init("..\\Engine\\Assets\\Shaders\\Gouraud.glsl");
}