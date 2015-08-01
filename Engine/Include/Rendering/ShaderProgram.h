#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <fstream>
#include <unordered_map>

#include "..\Scene\Resource.h"

class ResourceInfo;

using std::string;
using std::unordered_map;

class ShaderProgram : public Resource
{
public:

    ShaderProgram() {}
    ShaderProgram(string path, ResourceInfo* resourceInfo);

    void    Load(string path);
    void    ApplyShader();
	
    GLuint	GetID()	const;
    GLint	GetUniformLocation(string paramName);
    GLint	GetAttributeLocation(string paramName);

    string  GetUniformName(GLint paramID);
    string  GetAttributeName(GLint paramID);

    void	Delete();

private:
    bool    LoadShaderFromFile(string path);
    GLuint  CompileShader(string source, string path, GLenum type);
    void    LinkProgram();

    bool    IsShaderTypeDelimiter(string line);

    GLuint  m_programID;
    GLuint  m_vertexID;
    GLuint  m_fragmentID;

    unordered_map<string, GLint> m_cachedUniformLocations;
    unordered_map<string, GLint> m_cachedAttributeLocations;
};