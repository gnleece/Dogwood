#include "DebugDraw.h"
#include "..\Rendering\ShaderProgram.h"

void DebugDraw::Startup()
{
    m_shader = new ShaderProgram();
    m_shader->Load("Engine\\Shaders\\DebugDrawVertexShader.glsl", "Engine\\Shaders\\DebugDrawFragmentShader.glsl");

    glGenBuffers(1, &m_vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexBufferData), m_vertexBufferData, GL_DYNAMIC_DRAW);
    
    glGenBuffers(1, &m_colourBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_colourBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_lineColours), m_lineColours, GL_DYNAMIC_DRAW);

    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID); 
}

void DebugDraw::Shutdown()
{
    glDeleteBuffers(1, &m_vertexBufferID);
    glDeleteBuffers(1, &m_colourBufferID);
    glDeleteVertexArrays(1, &m_vertexArrayID);
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

void DebugDraw::RenderLines()
{
    m_shader->ApplyShader();

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

