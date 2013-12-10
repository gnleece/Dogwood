#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <fstream>

GLuint loadShaderFromFile( std::string path, GLenum shaderType );