#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <fstream>

#include "..\Scene\Resource.h"

class ResourceInfo;

using std::string;

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
    ShaderProgram(string path, ResourceInfo* resourceInfo);

    void    Load(string path);
    void    ApplyShader();
	
    GLuint	GetID()	const;
    GLint	GetParamLocation(eShaderParam param) const;
    void	Delete();

private:
    GLuint  m_programID;
    GLuint  m_vertexID;
    GLuint  m_fragmentID;

    GLint   m_paramLocations[NUM_PARAMS];

    bool    LoadShaderFromFile(string path);
    GLuint  CompileShader(string source, string path, GLenum type);
    void    LinkProgram();

    bool    IsShaderTypeDelimiter(string line);
};