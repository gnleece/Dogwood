#include "Rendering\OpenGL\DebugDrawImpl.h"
#include "Rendering\OpenGL\MaterialImpl.h"
#include "Rendering\OpenGL\RenderManagerImpl.h"
#include "Rendering\OpenGL\ShaderProgramImpl.h"
#include "Rendering\Material.h"
#include "Rendering\RenderManager.h"

DebugDraw* DebugDraw::Create()
{
    return new DebugDrawImpl();
}

void DebugDraw::Destroy(DebugDraw* debugDraw)
{
    delete debugDraw;
}

void DebugDrawImpl::Startup(RenderManagerImpl* renderManager)
{
    m_renderManager = renderManager;

    SetupDebugMat(renderManager);

    // Debug lines
    glGenBuffers(1, &m_vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexBufferData), m_vertexBufferData, GL_DYNAMIC_DRAW);

    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

    m_debugSphere.Init(renderManager, 1, 12);
    m_debugCube.Init(renderManager);
    m_debugCapsule.Init(renderManager, 1, 2, 12, AXIS_Y);
}

void DebugDrawImpl::Shutdown()
{
    glDeleteBuffers(1, &m_vertexBufferID);
    glDeleteBuffers(1, &m_ColorBufferID);
    glDeleteVertexArrays(1, &m_vertexArrayID);

    //TODO clean up material/shader
}

void DebugDrawImpl::DrawLine(Vector3& a, Vector3& b, ColorRGB& Color)
{
    if (m_numLines < MAX_LINES_PER_FRAME)
    {
        m_vertexBufferData[m_numLines*2] = a;
        m_vertexBufferData[m_numLines*2 + 1] = b;
        m_lineColors[m_numLines*2] = Color;
        m_lineColors[m_numLines*2 + 1] = Color;
        m_numLines++;
    }
}

//void DebugDraw::PrepareLineBuffer(Vector3* buffer, int count, GLuint& vao, GLuint &vbo)
//{
//    glGenVertexArrays(1, &vao);
//    glBindVertexArray(vao);
//
//    glGenBuffers(1, &vbo);
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*count * 3, buffer, GL_STATIC_DRAW);
//}
//
//void DebugDraw::DrawLineBuffer(GLuint vao, GLuint vbo, Vector3* buffer, int count, ColorRGB color)
//{
//    // Bind arrays/buffers
//    glBindVertexArray(vao);
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//
//    // Set the shader
//    m_shader->ApplyShader();
//
//    // Set uniform color value
//    GLint paramLocation = m_shader->GetUniformLocation("color");
//    glUniform3fv(paramLocation, 1, color.Start());
//
//    // Set model matrix to identity (debug lines are given in world coords)
//    GLint uniModel = m_shader->GetUniformLocation("model");
//    glUniformMatrix4fv(uniModel, 1, GL_FALSE, Matrix4x4::Identity.Transpose().Start());
//
//    // Bind position data
//    paramLocation = m_shader->GetAttributeLocation("position");
//    glEnableVertexAttribArray(paramLocation);
//    glVertexAttribPointer(paramLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//
//    // Draw the lines!
//    glDrawArrays(GL_LINES, 0, count);
//
//    glDisableVertexAttribArray(0);
//}

void DebugDrawImpl::RenderLines()
{
    m_shader->ApplyShader();

    // TODO this function is broken now that Colors are uniform parameters

    // Set model matrix to identity. debug lines are given in world coords
    GLint uniModel = m_shader->GetUniformLocation("model");
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, Matrix4x4::Identity.Transpose().Start());

    // Load position data
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexBufferData), m_vertexBufferData, GL_DYNAMIC_DRAW);

    GLint paramLocation = m_shader->GetAttributeLocation("position");
    glEnableVertexAttribArray(paramLocation);
    glVertexAttribPointer(paramLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    // Load Color data
    glBindBuffer(GL_ARRAY_BUFFER, m_ColorBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_lineColors), m_lineColors, GL_DYNAMIC_DRAW);

    paramLocation = m_shader->GetAttributeLocation("color");
    glEnableVertexAttribArray(paramLocation);
    glVertexAttribPointer(paramLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    // Draw the lines!
    glDrawArrays(GL_LINES, 0, m_numLines * 2);

    glDisableVertexAttribArray(0);
    m_numLines = 0;
}

void DebugDrawImpl::DrawSphere(Matrix4x4& transform, ColorRGB color, bool useDepth)
{
    m_debugSphere.Draw(transform, color, useDepth);
}

void DebugDrawImpl::DrawCube(Matrix4x4& transform, ColorRGB color, bool useDepth)
{
    m_debugCube.Draw(transform, color, useDepth);
}

void DebugDrawImpl::DrawCapsule(Matrix4x4& transform, ColorRGB color, bool useDepth)
{
    m_debugCapsule.Draw(transform, color, useDepth);
}

MaterialImpl* DebugDrawImpl::GetDebugMaterial()
{
    return m_material;
}

void DebugDrawImpl::SetupDebugMat(RenderManagerImpl* renderManager)
{
    m_shader = renderManager->GetCommonShader(RenderManager::eCommonShader::SHADER_UNLIT_UNI_COLOR);

    m_material = MaterialImpl::Create();
    m_material->SetShader(m_shader);
}

void Gnomon::Init(RenderManagerImpl* renderManager, float arrowBase, float arrowHeight)
{
    // Unit lines along x, y, z axes
    m_positionBufferData[0] = Vector3::Zero;
    m_positionBufferData[1] = Vector3(1.0f, 0.0f, 0.0f);
    m_positionBufferData[2] = Vector3::Zero;
    m_positionBufferData[3] = Vector3(0.0f, 1.0f, 0.0f);
    m_positionBufferData[4] = Vector3::Zero;
    m_positionBufferData[5] = Vector3(0.0f, 0.0f, 1.0f);

    // x : red, y : blue, z : green
    m_colorBufferData[0] = ColorRGB::Red;
    m_colorBufferData[1] = ColorRGB::Red;
    m_colorBufferData[2] = ColorRGB::Green;
    m_colorBufferData[3] = ColorRGB::Green;
    m_colorBufferData[4] = ColorRGB::Blue;
    m_colorBufferData[5] = ColorRGB::Blue;

    // Bind buffer data
    glGenBuffers(1, &m_positionBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_positionBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_positionBufferData), m_positionBufferData, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &m_colorBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_colorBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_colorBufferData), m_colorBufferData, GL_DYNAMIC_DRAW);

    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

    // Init arrows
    m_arrow.Init(renderManager, arrowBase, arrowHeight);
    m_arrowTransforms[0] = Rotation(90, eAXIS::AXIS_Y)*Translation(Vector3(0, 0, 1));
    m_arrowTransforms[1] = Rotation(-90, eAXIS::AXIS_X)*Translation(Vector3(0, 0, 1));
    m_arrowTransforms[2] = Rotation(-90, eAXIS::AXIS_Z)*Translation(Vector3(0, 0, 1))*Rotation(-30, eAXIS::AXIS_Z);

    // Get shader
    m_shader = renderManager->GetCommonShader(RenderManager::eCommonShader::SHADER_UNLIT);
}

void Gnomon::Draw(Transform& transform)
{
    // Disable depth so that gnomon renders on top of everything
    glDisable(GL_DEPTH_TEST);

    // Set the shader
    m_shader->ApplyShader();

    // Bind arrays/buffers
    glBindVertexArray(m_vertexArrayID);

    Transform scaledTransform = GetScaledTransform(transform);

    // Set model matrix
    GLint uniModel = m_shader->GetUniformLocation("model");
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, scaledTransform.GetWorldMatrix().Transpose().Start());

    // Bind position data
    GLint posParamLocation = m_shader->GetAttributeLocation("position");
    glEnableVertexAttribArray(posParamLocation);
    glBindBuffer(GL_ARRAY_BUFFER, m_positionBufferID);
    glVertexAttribPointer(posParamLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    // Bind color data
    GLint colParamLocation = m_shader->GetAttributeLocation("color");
    glEnableVertexAttribArray(colParamLocation);
    glBindBuffer(GL_ARRAY_BUFFER, m_colorBufferID);
    glVertexAttribPointer(colParamLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    // Draw the lines!
    glDrawArrays(GL_LINES, 0, 6);

    // Cleanup
    glDisableVertexAttribArray(posParamLocation);
    glDisableVertexAttribArray(colParamLocation);

    // Draw arrows!
    m_arrow.Draw(scaledTransform.GetWorldMatrix()*(m_arrowTransforms[0]), ColorRGB::Red);
    m_arrow.Draw(scaledTransform.GetWorldMatrix()*(m_arrowTransforms[1]), ColorRGB::Green);
    m_arrow.Draw(scaledTransform.GetWorldMatrix()*(m_arrowTransforms[2]), ColorRGB::Blue);

    // Re-enable depth
    glEnable(GL_DEPTH_TEST);
}

Transform Gnomon::GetScaledTransform(Transform& transform)
{
    // Scale the gnomon to be a (roughly) constant size in screen space
    Transform gnomonTransform(transform);
    Vector3 cameraPosition = RenderManager::Singleton()->GetCamera().GetPosition();
    Vector3 gnomonPosition = gnomonTransform.GetWorldPosition();
    float distance = Vector3::Distance(cameraPosition, gnomonPosition);
    Vector3 normalizedScale = 0.1f * distance * Vector3::One;
    gnomonTransform.SetLocalScale(normalizedScale);
    return gnomonTransform;
}

void DebugPrimitive::Draw(Matrix4x4& transform, ColorRGB& color, bool useDepth)
{
    if (!useDepth)
    {
        glDisable(GL_DEPTH_TEST);
    }

    m_shader->ApplyShader();

    glBindVertexArray(m_vertexArrayID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    // Set model matrix
    GLint uniModel = m_shader->GetUniformLocation("model");
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, transform.Transpose().Start());

    // Set uniform color
    GLint uniColor = m_shader->GetUniformLocation("color");
    glUniform3fv(uniColor, 1, color.Start());

    // Bind position data
    GLint paramLocation = m_shader->GetAttributeLocation("position");
    glEnableVertexAttribArray(paramLocation);
    glBindBuffer(GL_ARRAY_BUFFER, m_positionBufferID);
    glVertexAttribPointer(paramLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    glDrawElements(GL_LINES, sizeof(GLuint)*m_numIndices, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(paramLocation);

    if (!useDepth)
    {
        glEnable(GL_DEPTH_TEST);
    }
}

void DebugSphere::Init(RenderManagerImpl* renderManager, float radius, int divisions)
{
    // Calculate vertex positions
    int numPoints = divisions*divisions + 1;
    m_positionBufferData = new Vector3[numPoints];

    float angleDelta = MATH_PI / divisions;
    for (int i = 0; i < divisions; i++)
    {
        for (int j = 0; j < divisions; j++)
        {
            float theta = angleDelta * i;
            float phi = angleDelta * j * 2;
            float x = radius * sin(theta) * cos(phi);
            float y = radius * sin(theta) * sin(phi);
            float z = radius * cos(theta);
            Vector3 point = Vector3(x, y, z);
            m_positionBufferData[i*divisions + j] = point;
        }
    }
    m_positionBufferData[numPoints - 1] = Vector3(0, 0, -radius);

    // Populate index buffer
    m_numIndices = divisions * divisions * 2 * 2;     // numPoints * (horizontal + vertical) * 2 points per line
    m_indices = new GLuint[m_numIndices];

    int index = 0;
    for (int i = 0; i < divisions; i++)
    {
        for (int j = 0; j < divisions; j++)
        {
            int firstIndex = i*divisions + j;
            int secondIndex = firstIndex + 1;
            if (j == divisions - 1)
            {
                secondIndex -= divisions;
            }
            m_indices[index++] = firstIndex;
            m_indices[index++] = secondIndex;

            secondIndex = firstIndex + divisions;
            if (i == divisions - 1)
            {
                secondIndex = numPoints - 1;
            }
            m_indices[index++] = firstIndex;
            m_indices[index++] = secondIndex;

        }
    }

    // Bind buffer data
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

    // Upload vertex data
    glGenBuffers(1, &m_positionBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_positionBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3)*numPoints, m_positionBufferData, GL_STATIC_DRAW);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*m_numIndices, m_indices, GL_STATIC_DRAW);

    m_shader = renderManager->GetCommonShader(RenderManager::eCommonShader::SHADER_UNLIT_UNI_COLOR);
}

DebugSphere::~DebugSphere()
{
    delete[] m_positionBufferData;
    delete[] m_indices;
}

void DebugCube::Init(RenderManagerImpl* renderManager)
{
    // Calculate vertex positions
    int numPoints = 8;
    m_positionBufferData = new Vector3[numPoints];

    float size = 1.0f;
    m_positionBufferData[0] = Vector3(-size, -size, -size);
    m_positionBufferData[1] = Vector3(-size, -size,  size);
    m_positionBufferData[2] = Vector3(-size,  size, -size);
    m_positionBufferData[3] = Vector3(-size,  size,  size);
    m_positionBufferData[4] = Vector3( size, -size, -size);
    m_positionBufferData[5] = Vector3( size, -size,  size);
    m_positionBufferData[6] = Vector3( size,  size, -size);
    m_positionBufferData[7] = Vector3( size,  size,  size);

    // Populate index buffer
    m_numIndices = 12 * 2;                  // 12 lines * 2 points per line
    m_indices = new GLuint[m_numIndices];

    m_indices[0] = 0; m_indices[1] = 1;
    m_indices[2] = 2; m_indices[3] = 3;
    m_indices[4] = 4; m_indices[5] = 5;
    m_indices[6] = 6; m_indices[7] = 7;
    m_indices[8] = 0; m_indices[9] = 2;
    m_indices[10] = 1; m_indices[11] = 3;
    m_indices[12] = 4; m_indices[13] = 6;
    m_indices[14] = 5; m_indices[15] = 7;
    m_indices[16] = 0; m_indices[17] = 4;
    m_indices[18] = 1; m_indices[19] = 5;
    m_indices[20] = 2; m_indices[21] = 6;
    m_indices[22] = 3; m_indices[23] = 7;

    // Bind buffer data
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

    // Upload vertex data
    glGenBuffers(1, &m_positionBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_positionBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3)*numPoints, m_positionBufferData, GL_STATIC_DRAW);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*m_numIndices, m_indices, GL_STATIC_DRAW);

    m_shader = renderManager->GetCommonShader(RenderManager::eCommonShader::SHADER_UNLIT_UNI_COLOR);
}

DebugCube::~DebugCube()
{
    delete[] m_positionBufferData;
    delete[] m_indices;
}

void DebugCapsule::Init(RenderManagerImpl* renderManager, float radius, float height, int divisions, eAXIS axis)
{
    // The points always define the capsule along the y-axis, but we'll rotate them
    // in Draw() if the given axis is different
    m_axis = axis;

    // Calculate vertex positions
    int numPoints = divisions*divisions + 1;
    m_positionBufferData = new Vector3[numPoints];

    float angleDelta = MATH_PI / divisions;
    for (int i = 0; i < divisions; i++)
    {
        for (int j = 0; j < divisions + 1; j++)
        {
            float theta = angleDelta * i;
            float phi = angleDelta * j * 2;
            float offset = j <= divisions / 2 ? height / 2 : -height / 2;
            float x = radius * sin(theta) * cos(phi);
            float y = radius * sin(theta) * sin(phi) + offset;
            float z = radius * cos(theta);

            Vector3 point = Vector3(x, y, z);
            m_positionBufferData[i*divisions + j] = point;
        }
    }
    m_positionBufferData[numPoints - 1] = Vector3(0, 0, -radius);

    // Populate index buffer
    m_numIndices = divisions * divisions * 2 * 2;     // numPoints * (horizontal + vertical) * 2 points per line
    m_indices = new GLuint[m_numIndices];

    int index = 0;
    for (int i = 0; i < divisions; i++)
    {
        for (int j = 0; j < divisions; j++)
        {
            int firstIndex = i*divisions + j;
            int secondIndex = firstIndex + 1;
            if (j == divisions - 1)
            {
                secondIndex -= divisions;
            }
            m_indices[index++] = firstIndex;
            m_indices[index++] = secondIndex;

            secondIndex = firstIndex + divisions;
            if (i == divisions - 1)
            {
                secondIndex = numPoints - 1;
            }
            m_indices[index++] = firstIndex;
            m_indices[index++] = secondIndex;

        }
    }

    // Bind buffer data
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

    // Upload vertex data
    glGenBuffers(1, &m_positionBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_positionBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3)*numPoints, m_positionBufferData, GL_STATIC_DRAW);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*m_numIndices, m_indices, GL_STATIC_DRAW);

    m_shader = renderManager->GetCommonShader(RenderManager::eCommonShader::SHADER_UNLIT_UNI_COLOR);
}

void DebugCapsule::Draw(Matrix4x4& transform, ColorRGB& color, bool useDepth)
{
    if (m_axis == AXIS_X)
    {
        transform = transform * RotationEulerAngles(90*Vector3::Forward);
    }
    else if (m_axis == AXIS_Z)
    {
        transform = transform * RotationEulerAngles(90*Vector3::Right);
    }

    DebugPrimitive::Draw(transform, color, useDepth);
}

DebugCapsule::~DebugCapsule()
{
    delete[] m_positionBufferData;
    delete[] m_indices;
}

void Pyramid::Init(RenderManagerImpl* renderManager, float base, float height)
{
    // Calculate vertex positions
    float x = 0.866f*base;      // 0.866 = cos30
    float y = 0.5f*base;;       // 0.5 = sin30
    m_positionBufferData[0] = Vector3(0, base, 0);
    m_positionBufferData[1] = Vector3(-x, -y, 0);
    m_positionBufferData[2] = Vector3( x, -y, 0);
    m_positionBufferData[3] = Vector3(0, 0, height);

    // Populate index buffer
    m_indices[0] = 0; m_indices[1] = 1, m_indices[2] = 3;
    m_indices[3] = 1; m_indices[4] = 2, m_indices[5] = 3;
    m_indices[6] = 2; m_indices[7] = 0, m_indices[8] = 3;
    m_indices[9] = 2; m_indices[10] = 1, m_indices[11] = 0;

    // Bind buffer data
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

    // Upload vertex data
    glGenBuffers(1, &m_positionBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_positionBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_positionBufferData), m_positionBufferData, GL_STATIC_DRAW);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

    m_shader = renderManager->GetCommonShader(RenderManager::eCommonShader::SHADER_UNLIT_UNI_COLOR);
}

void Pyramid::Draw(Matrix4x4& transform, ColorRGB& color)
{
    m_shader->ApplyShader();

    glBindVertexArray(m_vertexArrayID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    // Set model matrix
    GLint uniModel = m_shader->GetUniformLocation("model");
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, transform.Transpose().Start());

    // Set uniform color
    GLint uniColor = m_shader->GetUniformLocation("color");
    glUniform3fv(uniColor, 1, color.Start());

    // Bind position data
    GLint paramLocation = m_shader->GetAttributeLocation("position");
    glEnableVertexAttribArray(paramLocation);
    glBindBuffer(GL_ARRAY_BUFFER, m_positionBufferID);
    glVertexAttribPointer(paramLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    glDrawElements(GL_TRIANGLES, sizeof(m_indices), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(paramLocation);
}