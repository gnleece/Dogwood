#include "Rendering\Light.h"

#include "Rendering\ShaderProgram.h"

Light::Light()
{
    position = Vector3(0.0f, 0.0f, 0.0f);
    color = ColorRGB(1.0f, 1.0f, 1.0f);
    power = 15.0f;
}

Light::Light(Vector3 p, ColorRGB c, GLfloat pow)
{
    position = p;
    color = c;
    power = pow;
}

void Light::ApplyLight(ShaderProgram* shader)
{
    if (shader)
    {
        GLint uniPosition = shader->GetUniformLocation("lightPos");
        glUniform3fv(uniPosition, 1, position.Start());

        GLint uniColor = shader->GetUniformLocation("lightColor");
        glUniform3fv(uniColor, 1, color.Start());

        GLint uniPower = shader->GetUniformLocation("lightPower");
        glUniform1f(uniPower, power);
    }
}