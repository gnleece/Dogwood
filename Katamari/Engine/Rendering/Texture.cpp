#include "Texture.h"

#include "Image.h"

Texture* Texture::m_defaultTexture = NULL;

Texture::Texture(std::string filename)
{
    m_filename = filename;

    ImageBMP image;
    image.Load(filename);

    // Create one OpenGL texture
    glGenTextures(1, &m_id);
 
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, m_id);
 
    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.Width(), image.Height(), 0, GL_BGR, GL_UNSIGNED_BYTE, image.DataPtr());
    image.FreeData();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::BindTexture()
{
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::FreeTexture()
{
    glDeleteTextures(1, &m_id);
}

Texture* Texture::DefaultTexture()
{
    printf("Grabbing default texture\n");
    if (m_defaultTexture)
    {
        return m_defaultTexture;
    }
    printf("Didn't exist, creating now...\n");
    m_defaultTexture = new Texture("Engine\\Assets\\default_texture.bmp");
    return m_defaultTexture;
}