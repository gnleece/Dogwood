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
    m_projMatrix.SetMatrix(projMatrix);
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

    m_clearColour = m_config.clearColour;

    // Common asset setup
    LoadCommonShaders();

    // Dependent manager setup
    DebugDraw::Singleton().Startup();
}

void RenderManager::Shutdown()
{
    DebugDraw::Singleton().Shutdown();
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
    m_viewMatrix.SetMatrix(LookAt(camera));
    m_dirty = true;
}

void RenderManager::SetView(Matrix4x4& view)
{
    m_viewMatrix.SetMatrix(view);
    m_dirty = true;
}

void RenderManager::SetClearColour(ColourRGB colour)
{
    m_clearColour = colour;
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
    glClearColor(m_clearColour.r, m_clearColour.g, m_clearColour.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render game objects
    if (m_rootObject != NULL)
    {
        m_rootObject->Render(Transform::Identity, false);
    }

    //DebugDraw::Singleton().RenderLines();

    // Swap buffers
    //m_gameWindow->SwapBuffers();  // TODO fixme qt-opengl-convert
}

void RenderManager::ApplyGlobalParams(ShaderProgram* shader)
{
    m_light.ApplyLight(shader);

    SetUniformMatrix(shader, ShaderProgram::UNI_VIEW, m_viewMatrix.GetMatrix());
    SetUniformMatrix(shader, ShaderProgram::UNI_PROJ, m_projMatrix.GetMatrix());

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
    Vector3 normalizedPosition = ((m_projMatrix*m_viewMatrix)*pos).xyz();
    float x = Clamp(normalizedPosition[0] / normalizedPosition[2], -1.f, 1.f);
    float y = Clamp(normalizedPosition[1] / normalizedPosition[2], -1.f, 1.f);
    screenPos[0] = (x + 1.0f) * m_config.width / 2.0f;
    screenPos[1] = (1.0f - y) * m_config.height / 2.0f;
    return screenPos;
}

void RenderManager::SetUniformMatrix(ShaderProgram* shader, ShaderProgram::eShaderParam param, Matrix4x4 & matrix)
{
    GLint paramLocation = shader->GetParamLocation(param);
    glUniformMatrix4fv(paramLocation, 1, GL_FALSE, matrix.Transpose().Start());
}

void RenderManager::LoadCommonShaders()
{
    m_commonShaders[SHADER_UNLIT] = new ShaderProgram();
    (m_commonShaders[SHADER_UNLIT])->Load("..\\Engine\\Assets\\Shaders\\Unlit.vert.glsl", "..\\Engine\\Assets\\Shaders\\Unlit.frag.glsl");

    m_commonShaders[SHADER_UNLIT_UNI_COLOR] = new ShaderProgram();
    (m_commonShaders[SHADER_UNLIT_UNI_COLOR])->Load("..\\Engine\\Assets\\Shaders\\UnlitUniformColor.vert.glsl", "..\\Engine\\Assets\\Shaders\\UnlitUniformColor.frag.glsl");

    m_commonShaders[SHADER_GOURAUD] = new ShaderProgram();
    (m_commonShaders[SHADER_GOURAUD])->Load("..\\Engine\\Assets\\Shaders\\Gouraud.vert.glsl", "..\\Engine\\Assets\\Shaders\\Gouraud.frag.glsl");
}