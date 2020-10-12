#ifdef GRAPHICS_GL

#include "Rendering\GL\GLTexture.h"

Texture* Texture::Create()
{
    return new GLTexture();
}

void Texture::Destroy(Texture* texture)
{
    delete texture;
}

Texture* Texture::DefaultTexture()
{
    if (m_defaultTexture == NULL)
    {
        m_defaultTexture = Create();
        m_defaultTexture->Init("..\\Engine\\Assets\\Textures\\default_texture.bmp", NULL);  // TODO ughhhh
    }
    
    return m_defaultTexture;
}

Texture* Texture::m_defaultTexture = NULL;

void GLTexture::Init(std::string filename, ResourceInfo* resourceInfo)
{
    m_filename = filename;
    m_resourceInfo = resourceInfo;

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

void GLTexture::BindTexture()
{
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void GLTexture::FreeTexture()
{
    glDeleteTextures(1, &m_id);
}

#endif
