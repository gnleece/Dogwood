#include "Rendering/GL/GLRenderer.h"
#include "Rendering/GL/GLShaderProgram.h"

void GLRenderer::ApplyLight(Light& light, ShaderProgram* shader)
{
    
    if (shader != NULL)
    {
        GLShaderProgram* shaderImpl = (GLShaderProgram*)shader;
        if (shaderImpl != NULL)
        {
            GLint uniPosition = shaderImpl->GetUniformLocation("lightPos");
            glUniform3fv(uniPosition, 1, light.position.Start());

            GLint uniColor = shaderImpl->GetUniformLocation("lightColor");
            glUniform3fv(uniColor, 1, light.color.Start());

            GLint uniPower = shaderImpl->GetUniformLocation("lightPower");
            glUniform1f(uniPower, light.power);
        }
    }
}