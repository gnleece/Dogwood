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