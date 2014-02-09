#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

class Image
{
public:
    virtual GLuint Load(const char* filename) = 0;
    // TODO add Save()
};

class ImageBMP : public Image
{
public:
    virtual GLuint Load(const char* filename);

private:

};