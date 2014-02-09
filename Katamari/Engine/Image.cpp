#include "Image.h"

#include <fstream>

#define BMP_HEADER_SIZE 54

GLuint ImageBMP::Load(const char* filename)
{
    // open file
    FILE* file = fopen(filename, "rb");
    if (!file)
    {
        printf("Error loading BMP: could not open file %s\n", filename);
        return 0;
    }

    // read header
    unsigned char header[BMP_HEADER_SIZE];
    int readSize = fread(header, 1, BMP_HEADER_SIZE, file);
    if (readSize != BMP_HEADER_SIZE || header[0] != 'B' || header[1] != 'M')
    {
        printf("Error loading BMP: invalid header in %s\n", filename);
        return 0;
    }
    unsigned int dataPos    = *(int*)&(header[0x0A]);
    unsigned int width      = *(int*)&(header[0x12]);
    unsigned int height     = *(int*)&(header[0x16]);
    unsigned int size       = *(int*)&(header[0x22]);

    if (size == 0)
    {
        size = width*height*3;
    }
    if (dataPos == 0)
    {
        dataPos = BMP_HEADER_SIZE;
    }

    // read image data
    unsigned char* data = new unsigned char[size];
    fread(data, 1, size, file);
    fclose(file);
}