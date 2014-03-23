#include "Light.h"

void Light::SetLightForShader(GLuint shaderID)
{
    GLint uniPosition = glGetUniformLocation(shaderID, "lightPos");
    glUniform3fv(uniPosition, 1, position.Start());

    GLint uniColor = glGetUniformLocation(shaderID, "lightColor");
    glUniform3fv(uniColor, 1, color.Start());

    GLint uniPower = glGetUniformLocation(shaderID, "lightPower");
    glUniform1f(uniPower, power);
}