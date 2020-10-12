#pragma once

#ifdef GRAPHICS_GL

#include "Rendering\Texture.h"
#include "Rendering\Image.h"

#define GLEW_STATIC
#include <GL/glew.h>

class GLTexture : public Texture
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

#endif