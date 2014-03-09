#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <fstream>

class ShaderProgram
{
public:
    enum eAttribType { ATTRIB_POS, ATTRIB_NORMAL, ATTRIB_COLOR, ATTRIB_TEXCOORD, NUM_ATTRIBS };
	
    ShaderProgram(std::string vertexShaderPath, std::string fragmentShaderPath);
	
    GLuint	GetID()	const	{ return m_programID; }
    GLint	GetAttribLocation(eAttribType attrib) const;
    void	Delete();

private:
    GLuint m_programID;
    GLuint m_vertexID;
    GLuint m_fragmentID;

    GLint m_attribLocations[NUM_ATTRIBS];

    GLuint LoadShaderFromFile(std::string path, GLenum shaderType);
    GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);
};