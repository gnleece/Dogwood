#include "Rendering\Material.h"

#include "Rendering\RenderManager.h"
#include "Rendering\ShaderProgram.h"
#include "Rendering\Texture.h"

Material::Material()
{
    m_colours[MAT_COLOUR_DIFFUSE] = ColourRGB::White;
    m_colours[MAT_COLOUR_AMBIENT] = ColourRGB(0.1f, 0.1f, 0.1f);
    m_colours[MAT_COLOUR_SPECULAR] = ColourRGB::White;
}

Material* Material::DeepCopy()
{
    Material* newMaterial = new Material();

    newMaterial->m_shader = m_shader;
    newMaterial->m_texture = m_texture;
    newMaterial->m_colours[MAT_COLOUR_DIFFUSE] = m_colours[MAT_COLOUR_DIFFUSE];
    newMaterial->m_colours[MAT_COLOUR_AMBIENT] = m_colours[MAT_COLOUR_AMBIENT];
    newMaterial->m_colours[MAT_COLOUR_SPECULAR] = m_colours[MAT_COLOUR_SPECULAR];

    return newMaterial;
}

void Material::SetShader(ShaderProgram* shader)
{
    m_shader = shader;
}

void Material::SetTexture(Texture* texture)
{
    m_texture = texture;
}

void Material::SetColour(eMatColourType type, ColourRGB colour)
{
    if (type < NUM_MAT_COLOURS)
    {
        m_colours[type] = colour;
    }
}

ShaderProgram* Material::GetShader()
{
    return m_shader;
}

Texture* Material::GetTexture()
{
    return m_texture;
}

ColourRGB Material::GetColour(eMatColourType type)
{
    if (type < NUM_MAT_COLOURS)
    {
        return m_colours[type];
    }
    return ColourRGB::Black;
}

void Material::ApplyMaterial(GLint posVBO, GLint normVBO, GLint uvVBO, Transform& transform)
{
    // Apply shader
    m_shader->ApplyShader();

    // Bind texture
    if (m_texture == NULL)
    {
        m_texture = Texture::DefaultTexture();
    }
    m_texture->BindTexture();

    // Set uniform colour values for shader
    SetUniformParam(ShaderProgram::UNI_COLOUR_DIFFUSE,  MAT_COLOUR_DIFFUSE);
    SetUniformParam(ShaderProgram::UNI_COLOUR_AMBIENT,  MAT_COLOUR_AMBIENT);
    SetUniformParam(ShaderProgram::UNI_COLOUR_SPECULAR, MAT_COLOUR_SPECULAR);

    // Set vertex values for shader
    SetAttribParam(ShaderProgram::ATTRIB_POS,      posVBO,  3);
    SetAttribParam(ShaderProgram::ATTRIB_NORMAL,   normVBO, 3);
    SetAttribParam(ShaderProgram::ATTRIB_TEXCOORD, uvVBO,   2);

    // Set model matrix value for shader        // TODO not sure this should be here
    GLint uniModel = m_shader->GetParamLocation(ShaderProgram::UNI_MODEL);
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, transform.GetMatrix().Transpose().Start());
}

void Material::UnapplyMaterial()
{
    DisableAttribArray(ShaderProgram::ATTRIB_POS);
    DisableAttribArray(ShaderProgram::ATTRIB_NORMAL);
    DisableAttribArray(ShaderProgram::ATTRIB_TEXCOORD);
}

void Material::SetUniformParam(ShaderProgram::eShaderParam param, eMatColourType colour)
{
    GLint paramLocation = m_shader->GetParamLocation(param);
    glUniform3fv(paramLocation, 1, m_colours[colour].Start());
}

void Material::SetAttribParam(ShaderProgram::eShaderParam param, GLint buffer, int size)
{
    GLint paramLocation = m_shader->GetParamLocation(param);
    glEnableVertexAttribArray(paramLocation);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(paramLocation, size, GL_FLOAT, GL_FALSE, size * sizeof(float), 0);
}

void Material::DisableAttribArray(ShaderProgram::eShaderParam param)
{
    GLint paramLocation = m_shader->GetParamLocation(param);
    glDisableVertexAttribArray(paramLocation);
}