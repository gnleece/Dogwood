#include "Debugging\DebugDraw.h"
#include "Rendering\Material.h"
#include "Rendering\RenderManager.h"
#include "Rendering\ShaderProgram.h"

void DebugDraw::Startup()
{
    SetupDebugMat();

    // Debug lines
    glGenBuffers(1, &m_vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexBufferData), m_vertexBufferData, GL_DYNAMIC_DRAW);

    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);
}

void DebugDraw::Shutdown()
{
    glDeleteBuffers(1, &m_vertexBufferID);
    glDeleteBuffers(1, &m_colourBufferID);
    glDeleteVertexArrays(1, &m_vertexArrayID);

    //TODO clean up material/shader
}

void DebugDraw::DrawLine(Vector3& a, Vector3& b, ColourRGB& colour)
{
    if (m_numLines < MAX_LINES_PER_FRAME)
    {
        m_vertexBufferData[m_numLines*2] = a;
        m_vertexBufferData[m_numLines*2 + 1] = b;
        m_lineColours[m_numLines*2] = colour;
        m_lineColours[m_numLines*2 + 1] = colour;
        m_numLines++;
    }
}

void DebugDraw::PrepareLineBuffer(Vector3* buffer, int count, GLuint& vao, GLuint &vbo)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*count * 3, buffer, GL_STATIC_DRAW);
}

void DebugDraw::DrawLineBuffer(GLuint vao, GLuint vbo, Vector3* buffer, int count, ColourRGB color)
{
    // Bind arrays/buffers
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Set the shader
    m_shader->ApplyShader();

    // Set uniform color value
    GLint paramLocation = m_shader->GetParamLocation(ShaderProgram::UNI_COLOUR_DIFFUSE);
    glUniform3fv(paramLocation, 1, color.Start());

    // Set model matrix to identiy (debug lines are given in world coords)
    GLint uniModel = m_shader->GetParamLocation(ShaderProgram::UNI_MODEL);
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, Matrix4x4::Identity.Transpose().Start());

    // Bind position data
    paramLocation = m_shader->GetParamLocation(ShaderProgram::ATTRIB_POS);
    glEnableVertexAttribArray(paramLocation);
    glVertexAttribPointer(paramLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    // Draw the lines!
    glDrawArrays(GL_LINES, 0, count);

    glDisableVertexAttribArray(0);
}

void DebugDraw::RenderLines()
{
    m_shader->ApplyShader();

    // TODO this function is broken now that colours are uniform parameters

    // Set model matrix to identiy. debug lines are given in world coords
    GLint uniModel = m_shader->GetParamLocation(ShaderProgram::UNI_MODEL);
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, Matrix4x4::Identity.Transpose().Start());

    // Load position data
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexBufferData), m_vertexBufferData, GL_DYNAMIC_DRAW);

    GLint paramLocation = m_shader->GetParamLocation(ShaderProgram::ATTRIB_POS);
    glEnableVertexAttribArray(paramLocation);
    glVertexAttribPointer(paramLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    // Load colour data
    glBindBuffer(GL_ARRAY_BUFFER, m_colourBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_lineColours), m_lineColours, GL_DYNAMIC_DRAW);

    paramLocation = m_shader->GetParamLocation(ShaderProgram::ATTRIB_COLOUR);
    glEnableVertexAttribArray(paramLocation);
    glVertexAttribPointer(paramLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    // Draw the lines!
    glDrawArrays(GL_LINES, 0, m_numLines * 2);

    glDisableVertexAttribArray(0);
    m_numLines = 0;
}

Material* DebugDraw::GetDebugMaterial()
{
    return m_material;
}

void DebugDraw::SetupDebugMat()
{
    m_shader = RenderManager::Singleton().GetCommonShader(RenderManager::eCommonShader::SHADER_UNLIT_UNI_COLOR);

    m_material = new Material();
    m_material->SetShader(m_shader);
}

void Gnomon::Init(float arrowBase, float arrowHeight)
{
    // Unit lines along x, y, z axes
    m_positionBufferData[0] = Vector3::Zero;
    m_positionBufferData[1] = Vector3(1.0f, 0.0f, 0.0f);
    m_positionBufferData[2] = Vector3::Zero;
    m_positionBufferData[3] = Vector3(0.0f, 1.0f, 0.0f);
    m_positionBufferData[4] = Vector3::Zero;
    m_positionBufferData[5] = Vector3(0.0f, 0.0f, 1.0f);

    // x : red, y : blue, z : green
    m_colorBufferData[0] = ColourRGB::Red;
    m_colorBufferData[1] = ColourRGB::Red;
    m_colorBufferData[2] = ColourRGB::Blue;
    m_colorBufferData[3] = ColourRGB::Blue;
    m_colorBufferData[4] = ColourRGB::Green;
    m_colorBufferData[5] = ColourRGB::Green;

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
    m_arrow.Init(arrowBase, arrowHeight);
    m_arrowTransforms[0] = Rotation(90, eAXIS::AXIS_Y)*Translation(Vector3(0, 0, 1));
    m_arrowTransforms[1] = Rotation(-90, eAXIS::AXIS_X)*Translation(Vector3(0, 0, 1));
    m_arrowTransforms[2] = Rotation(-90, eAXIS::AXIS_Z)*Translation(Vector3(0, 0, 1))*Rotation(-30, eAXIS::AXIS_Z);

    // Get shader
    m_shader = RenderManager::Singleton().GetCommonShader(RenderManager::eCommonShader::SHADER_UNLIT);
}

void Gnomon::Draw(Matrix4x4& transform)
{
    // Disable depth so that gnomon renders on top of everything
    glDisable(GL_DEPTH_TEST);

    // Set the shader
    m_shader->ApplyShader();

    // Bind arrays/buffers
    glBindVertexArray(m_vertexArrayID);

    // Set model matrix
    GLint uniModel = m_shader->GetParamLocation(ShaderProgram::UNI_MODEL);
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, transform.Transpose().Start());

    // Bind position data
    GLint posParamLocation = m_shader->GetParamLocation(ShaderProgram::ATTRIB_POS);
    glEnableVertexAttribArray(posParamLocation);
    glBindBuffer(GL_ARRAY_BUFFER, m_positionBufferID);
    glVertexAttribPointer(posParamLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    // Bind color data
    GLint colParamLocation = m_shader->GetParamLocation(ShaderProgram::ATTRIB_COLOUR);
    glEnableVertexAttribArray(colParamLocation);
    glBindBuffer(GL_ARRAY_BUFFER, m_colorBufferID);
    glVertexAttribPointer(colParamLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    // Draw the lines!
    glDrawArrays(GL_LINES, 0, 6);

    // Cleanup
    glDisableVertexAttribArray(posParamLocation);
    glDisableVertexAttribArray(colParamLocation);

    // Draw arrows!
    m_arrow.Draw(transform*(m_arrowTransforms[0]), ColourRGB::Red);
    m_arrow.Draw(transform*(m_arrowTransforms[1]), ColourRGB::Blue);
    m_arrow.Draw(transform*(m_arrowTransforms[2]), ColourRGB::Green);

    // Re-enable depth
    glEnable(GL_DEPTH_TEST);
}

void Pyramid::Init(float base, float height)
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

    m_shader = RenderManager::Singleton().GetCommonShader(RenderManager::eCommonShader::SHADER_UNLIT_UNI_COLOR);
}

void Pyramid::Draw(Matrix4x4& transform, ColourRGB& color)
{
    m_shader->ApplyShader();

    glBindVertexArray(m_vertexArrayID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    // Set model matrix
    GLint uniModel = m_shader->GetParamLocation(ShaderProgram::UNI_MODEL);
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, transform.Transpose().Start());

    // Set uniform color
    GLint uniColor = m_shader->GetParamLocation(ShaderProgram::UNI_COLOUR_DIFFUSE);
    glUniform3fv(uniColor, 1, color.Start());

    // Bind position data
    GLint paramLocation = m_shader->GetParamLocation(ShaderProgram::ATTRIB_POS);
    glEnableVertexAttribArray(paramLocation);
    glBindBuffer(GL_ARRAY_BUFFER, m_positionBufferID);
    glVertexAttribPointer(paramLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    glDrawElements(GL_TRIANGLES, sizeof(m_indices), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(paramLocation);
}