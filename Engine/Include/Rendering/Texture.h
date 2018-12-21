#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <fstream>

#include "Scene\Resource.h"

class ResourceInfo;

class Texture : public Resource
{
public:
    Texture(std::string filename, ResourceInfo* resourceInfo);

    void BindTexture();
    void FreeTexture();

    static Texture* DefaultTexture();

private:
    GLuint          m_id;
    std::string     m_filename;

    static Texture* m_defaultTexture;
};