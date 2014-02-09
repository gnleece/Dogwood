#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

class Image
{
public:
    virtual bool Load(const char* filename) = 0;
    virtual void FreeData() = 0;
    // TODO add Save()

    unsigned int Width()    { return m_width; }
    unsigned int Height()   { return m_height; }
    unsigned int Size()     { return m_size; }

protected:
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_size;
};

class ImageBMP : public Image
{
public:
    ImageBMP();
    ImageBMP(const char* filename);
    ~ImageBMP();

    virtual bool            Load(const char* filename);
    virtual void            FreeData();

    const unsigned char*    DataPtr() { return m_data; }

private:
    bool            m_loaded;

    unsigned char*  m_data;
    unsigned int    m_dataPos;
};