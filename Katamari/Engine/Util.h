#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <fstream>

GLuint LoadShaderFromFile(std::string path, GLenum shaderType);
GLuint LinkShaderProgram(GLuint vertexShader, GLuint fragmentShader);
