#pragma once

#ifdef GRAPHICS_GL

#define GLEW_STATIC
#include <GL/glew.h>

#include "Rendering\ShaderProgram.h"

#include <unordered_map>

using std::unordered_map;

class GLShaderProgram : public ShaderProgram
{
public:
    friend class ShaderProgram;

    static GLShaderProgram* Create();

    virtual void Init(string path, ResourceInfo* resourceInfo = NULL);
    virtual void ApplyShader();
    virtual void Delete();

    GLuint	GetID()	const;
    GLint	GetUniformLocation(string paramName);
    GLint	GetAttributeLocation(string paramName);

    string  GetUniformName(GLint paramID);
    string  GetAttributeName(GLint paramID);

private:
    bool    LoadShaderFromFile(string path);
    GLuint  CompileShader(string source, string path, GLenum type);
    void    LinkProgram();

    bool    IsShaderTypeDelimiter(string line);

    void    ApplyCamera();
    void    ApplyLight();

    GLuint  m_programID;
    GLuint  m_vertexID;
    GLuint  m_fragmentID;

    unordered_map<string, GLint> m_cachedUniformLocations;
    unordered_map<string, GLint> m_cachedAttributeLocations;
};

#endif