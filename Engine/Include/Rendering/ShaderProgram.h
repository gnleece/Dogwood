#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <fstream>

#include "..\Scene\Resource.h"

class ResourceInfo;

class ShaderProgram : public Resource
{
public:

    enum eShaderParam
    {
        ATTRIB_POS,
        ATTRIB_NORMAL,
        ATTRIB_TEXCOORD,
        ATTRIB_COLOUR,
        UNI_MODEL,
        UNI_VIEW,
        UNI_PROJ,
        UNI_LIGHT_POS,
        UNI_LIGHT_COLOUR,
        UNI_LIGHT_POWER,
        UNI_COLOUR_DIFFUSE,
        UNI_COLOUR_AMBIENT,
        UNI_COLOUR_SPECULAR,
        NUM_PARAMS
    };
	
    ShaderProgram() {}
    ShaderProgram(std::string vertexShaderPath, std::string fragmentShaderPath, ResourceInfo* resourceInfo);

    void    Load(std::string vertexShaderPath, std::string fragmentShaderPath);
    void    ApplyShader();
	
    GLuint	GetID()	const	{ return m_programID; }
    GLint	GetParamLocation(eShaderParam param) const;
    void	Delete();

private:
    GLuint m_programID;
    GLuint m_vertexID;
    GLuint m_fragmentID;

    GLint m_paramLocations[NUM_PARAMS];

    GLuint LoadShaderFromFile(std::string path, GLenum shaderType);
    GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);
};