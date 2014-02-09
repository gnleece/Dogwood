#include "Texture.h"

#include "Image.h"

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
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, image.Width(), image.Height(), 0, GL_BGR, GL_UNSIGNED_BYTE, image.DataPtr());
    image.FreeData();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}