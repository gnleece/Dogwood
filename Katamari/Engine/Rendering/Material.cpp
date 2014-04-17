#include "Material.h"

#include "Texture.h"

Material::Material()
{
    m_colours[MAT_COLOUR_DIFFUSE] = ColourRGB::White;
    m_colours[MAT_COLOUR_AMBIENT] = ColourRGB(0.1f, 0.1f, 0.1f);
    m_colours[MAT_COLOUR_SPECULAR] = ColourRGB::White;
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

void Material::ApplyMaterial(GLint uniDiffuse, GLint uniAmbient, GLint uniSpecular)
{
    if (m_texture == NULL)
    {
        m_texture = Texture::DefaultTexture();
    }
    m_texture->BindTexture();

    glUniform3fv(uniDiffuse,  1, m_colours[MAT_COLOUR_DIFFUSE].Start());
    glUniform3fv(uniAmbient,  1, m_colours[MAT_COLOUR_AMBIENT].Start());
    glUniform3fv(uniSpecular, 1, m_colours[MAT_COLOUR_SPECULAR].Start());
}