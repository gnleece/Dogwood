#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <fstream>

class ShaderProgram
{
public:
	ShaderProgram(std::string vertexShaderPath, std::string fragmentShaderPath);
	
	GLuint GetID() { return m_programID; }

private:
	GLuint m_programID;
	GLuint m_vertexID;
	GLuint m_fragmentID;

	GLuint LoadShaderFromFile(std::string path, GLenum shaderType);
	GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);
};