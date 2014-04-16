#include "Material.h"

#include "Texture.h"

Material::Material()
{
    for (int i = 0; i < NUM_MAT_COLOURS; i++)
    {
        m_colours[i] = ColourRGB::White;
    }
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

void Material::ApplyMaterial(GLint uniColour)
{
    if (m_texture == NULL)
    {
        m_texture = Texture::DefaultTexture();
    }
    m_texture->BindTexture();

    glUniform3fv(uniColour, 1, m_colours[MAT_COLOUR_DIFFUSE].Start()); // TODO expose ambient and specular colours
}