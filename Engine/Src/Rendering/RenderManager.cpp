#include "Rendering\RenderManager.h"

#include "GameObject.h"
#include "Debugging\DebugDraw.h"

void RenderManager::Startup(RenderConfig& config)
{
    m_dirty = true;
    m_rootObject = NULL;
    m_config = config;

    // Prepare projection matrix
    float aspect = (float)m_config.width / m_config.height;
    Matrix4x4 projMatrix = PerspectiveProjection(m_config.FOV, aspect, m_config.nearPlane, m_config.farPlane);
    m_projMatrix.SetLocalMatrix(projMatrix);
    glViewport(0, 0, m_config.width, m_config.height);

    // OpenGL setup
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    glewExperimental = GL_TRUE;
    GLenum ret = glewInit();
    if (ret != 0)
    {
        printf("Error in glewInit! Abort.\n");
    }

    m_clearColor = m_config.clearColor;

    // Common asset setup
    LoadCommonShaders();

    // Dependent manager setup
    DebugDraw::Singleton().Startup();
}

void RenderManager::Shutdown()
{
    DebugDraw::Singleton().Shutdown();
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

void RenderManager::SetCamera(Camera camera)
{
    m_viewMatrix.SetLocalMatrix(LookAt(camera));
    m_dirty = true;
}

void RenderManager::SetView(Matrix4x4& view)
{
    m_viewMatrix.SetLocalMatrix(view);
    m_dirty = true;
}

void RenderManager::SetClearColor(ColorRGB Color)
{
    m_clearColor = Color;
}

Transform& RenderManager::GetView()
{
    return m_viewMatrix;
}

Transform& RenderManager::GetProjection()
{
    return m_projMatrix;
}

RenderConfig& RenderManager::GetConfig()
{
    return m_config;
}

void RenderManager::RenderScene()
{
    // Clear the screen to black
    glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render game objects
    if (m_rootObject != NULL)
    {
        m_rootObject->Render(false);
    }

    //DebugDraw::Singleton().RenderLines();

    // Swap buffers
    //m_gameWindow->SwapBuffers();  // TODO fixme qt-opengl-convert
}

void RenderManager::ApplyGlobalParams(ShaderProgram* shader)
{
    // Light
    m_light.ApplyLight(shader);

    // View matrix
    GLint viewLocation = shader->GetUniformLocation("view");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, m_viewMatrix.GetLocalMatrix().Transpose().Start());

    // Projection matrix
    GLint projLocation = shader->GetUniformLocation("proj");
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, m_projMatrix.GetLocalMatrix().Transpose().Start());

    m_dirty = false;
}

bool RenderManager::SettingsDirty()
{
    return m_dirty;
}

ShaderProgram* RenderManager::GetCommonShader(eCommonShader name)
{
    if (name >= 0 && name < NUM_COMMON_SHADERS)
        return m_commonShaders[name];
    return NULL;
}

Vector2 RenderManager::ToScreenSpace(Vector3 worldPosition)
{
    // TODO the math for this doesn't seem quite right, debug it
    Vector2 screenPos;
    Vector4 pos = (Vector4(worldPosition, 1));
    Vector3 normalizedPosition = ((m_projMatrix.GetWorldMatrix()*m_viewMatrix.GetWorldMatrix())*pos).xyz();
    float x = Clamp(normalizedPosition[0] / normalizedPosition[2], -1.f, 1.f);
    float y = Clamp(normalizedPosition[1] / normalizedPosition[2], -1.f, 1.f);
    screenPos[0] = (x + 1.0f) * m_config.width / 2.0f;
    screenPos[1] = (1.0f - y) * m_config.height / 2.0f;
    return screenPos;
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