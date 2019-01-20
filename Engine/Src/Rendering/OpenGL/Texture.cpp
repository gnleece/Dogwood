#include "Rendering\Texture.h"
#include "Rendering\Image.h"

#include <fstream>

#define GLEW_STATIC
#include <GL/glew.h>

class TextureImpl : public Texture
{
public:
    friend class Texture;

    virtual void    Init(std::string filename, ResourceInfo* resourceInfo);

    virtual void    BindTexture();
    virtual void    FreeTexture();

private:
    GLuint          m_id;
    std::string     m_filename;

    static Texture* m_defaultTexture;
};

Texture* Texture::Create()
{
    return new TextureImpl();
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

void TextureImpl::Init(std::string filename, ResourceInfo* resourceInfo)
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

void TextureImpl::BindTexture()
{
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void TextureImpl::FreeTexture()
{
    glDeleteTextures(1, &m_id);
}

