#include "Light.h"

#include "ShaderProgram.h"

Light::Light()
{
    position = Vector3(0.0f, 0.0f, 0.0f);
    color = ColourRGB(1.0f, 1.0f, 1.0f);
    power = 15.0f;
}

Light::Light(Vector3 p, ColourRGB c, GLfloat pow)
{
    position = p;
    color = c;
    power = pow;
}

void Light::ApplyLight(ShaderProgram* shader)
{
    if (shader)
    {
        GLint uniPosition = shader->GetParamLocation(ShaderProgram::UNI_LIGHT_POS);
        glUniform3fv(uniPosition, 1, position.Start());

        GLint uniColor = shader->GetParamLocation(ShaderProgram::UNI_LIGHT_COLOUR);
        glUniform3fv(uniColor, 1, color.Start());

        GLint uniPower = shader->GetParamLocation(ShaderProgram::UNI_LIGHT_POWER);
        glUniform1f(uniPower, power);
    }
}