#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <fstream>

class Texture
{
public:
    Texture(std::string filename);

private:
    GLuint      m_id;
    std::string m_filename;
};