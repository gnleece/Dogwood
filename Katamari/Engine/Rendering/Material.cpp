#include "Material.h"

#include "ShaderProgram.h"
#include "Texture.h"

Material::Material()
{
    m_colours[MAT_COLOUR_DIFFUSE] = ColourRGB::White;
    m_colours[MAT_COLOUR_AMBIENT] = ColourRGB(0.1f, 0.1f, 0.1f);
    m_colours[MAT_COLOUR_SPECULAR] = ColourRGB::White;
}

void Material::SetShader(ShaderProgram* shader)
{
    m_shader = shader;

    m_uniColourDiffuse = m_shader->GetParamLocation(ShaderProgram::UNI_COLOUR_DIFFUSE);
    m_uniColourAmbient = m_shader->GetParamLocation(ShaderProgram::UNI_COLOUR_AMBIENT);
    m_uniColourSpecular = m_shader->GetParamLocation(ShaderProgram::UNI_COLOUR_SPECULAR);
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

void Material::ApplyMaterial()
{
    if (m_texture == NULL)
    {
        m_texture = Texture::DefaultTexture();
    }
    m_texture->BindTexture();

    glUniform3fv(m_uniColourDiffuse,  1, m_colours[MAT_COLOUR_DIFFUSE].Start());
    glUniform3fv(m_uniColourAmbient,  1, m_colours[MAT_COLOUR_AMBIENT].Start());
    glUniform3fv(m_uniColourSpecular, 1, m_colours[MAT_COLOUR_SPECULAR].Start());
}