#include "Image.h"

#include <fstream>

#define BMP_HEADER_SIZE 54

ImageBMP::ImageBMP() : m_loaded(false) { }

ImageBMP::ImageBMP(std::string filename) : m_loaded(false)
{
    Load(filename);
}

ImageBMP::~ImageBMP()
{
    FreeData();
}

bool ImageBMP::Load(std::string filename)
{
    FreeData();
    
    // open file
    FILE* file;
    errno_t ret = fopen_s(&file, filename.c_str(), "rb");
    if (ret)
    {
        printf("Error loading BMP: could not open file %s\n", filename.c_str());
        return false;
    }

    // read header
    unsigned char header[BMP_HEADER_SIZE];
    int readSize = fread(header, 1, BMP_HEADER_SIZE, file);
    if (readSize != BMP_HEADER_SIZE || header[0] != 'B' || header[1] != 'M')
    {
        printf("Error loading BMP: invalid header in %s\n", filename);
        return false;
    }
    m_dataPos    = *(int*)&(header[0x0A]);
    m_width      = *(int*)&(header[0x12]);
    m_height     = *(int*)&(header[0x16]);
    m_size       = *(int*)&(header[0x22]);

    if (m_size == 0)
    {
        m_size = m_width*m_height*3;
    }
    if (m_dataPos == 0)
    {
        m_dataPos = BMP_HEADER_SIZE;
    }

    // read image data
    m_data = new unsigned char[m_size];
    fread(m_data, 1, m_size, file);
    fclose(file);

    m_loaded = true;
    return true;
}

void ImageBMP::FreeData()
{
    if (m_loaded)
    {
        delete [] m_data;

        m_width = 0;
        m_height = 0;
        m_size = 0;
        m_loaded = false;
    }
}