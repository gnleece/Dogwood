#include "Rendering/OpenGL/GLRenderer.h"
#include "Rendering/ShaderProgram.h"

void GLRenderer::ApplyLight(Light& light, ShaderProgram* shader)
{
    if (shader)
    {
        GLint uniPosition = shader->GetUniformLocation("lightPos");
        glUniform3fv(uniPosition, 1, light.position.Start());

        GLint uniColor = shader->GetUniformLocation("lightColor");
        glUniform3fv(uniColor, 1, light.color.Start());

        GLint uniPower = shader->GetUniformLocation("lightPower");
        glUniform1f(uniPower, light.power);
    }
}