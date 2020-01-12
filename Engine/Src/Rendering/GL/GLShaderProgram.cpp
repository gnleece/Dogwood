#include "Rendering/ShaderProgram.h"
#include "Rendering/GL/GLShaderProgram.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "Rendering\RenderManager.h"

ShaderProgram* ShaderProgram::Create()
{
    return GLShaderProgram::Create();
}

void ShaderProgram::Destroy(ShaderProgram* shader)
{
    delete shader;
}

GLShaderProgram* GLShaderProgram::Create()
{
    return new GLShaderProgram();
}

void GLShaderProgram::Init(string path, ResourceInfo* resourceInfo)
{
    m_resourceInfo = resourceInfo;

    //TODO check if already loaded and delete
    bool success = LoadShaderFromFile(path);
    if (success)
    {
        LinkProgram();
    }
}

void GLShaderProgram::ApplyShader()
{
    // Enable shader program if not already active
    bool changedProgram = false;
    GLint currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&currentProgram);
    if (currentProgram != GetID())
    {
        glUseProgram(GetID());
        changedProgram = true;
    }

    if (changedProgram || RenderManager::Singleton()->SettingsDirty())
    {
        RenderManager::Singleton()->ApplyGlobalParams(this);
    }
}

GLuint GLShaderProgram::GetID() const
{
    return m_programID;
}

GLint GLShaderProgram::GetUniformLocation(string param)
{
    if (m_cachedUniformLocations.count(param) == 0)
    {
        m_cachedUniformLocations[param] = glGetUniformLocation(m_programID, param.c_str());;
    }
    return m_cachedUniformLocations[param];
}

GLint GLShaderProgram::GetAttributeLocation(string param)
{
    if (m_cachedAttributeLocations.count(param) == 0)
    {
        m_cachedAttributeLocations[param] = glGetAttribLocation(m_programID, param.c_str());;
    }
    return m_cachedAttributeLocations[param];
}

string GLShaderProgram::GetUniformName(GLint paramID)
{
    // This lookup is slow, but should only be needed during scene serialization
    unordered_map<string, GLint>::iterator iter = m_cachedUniformLocations.begin();
    for (; iter != m_cachedUniformLocations.end(); iter++)
    {
        if (iter->second == paramID)
        {
            return iter->first;
        }
    }
    return "";
}

string GLShaderProgram::GetAttributeName(GLint paramID)
{
    // This lookup is slow, but should only be needed during scene serialization
    unordered_map<string, GLint>::iterator iter = m_cachedAttributeLocations.begin();
    for (; iter != m_cachedAttributeLocations.end(); iter++)
    {
        if (iter->second == paramID)
        {
            return iter->first;
        }
    }
    return "";
}

void GLShaderProgram::Delete()
{
    glDeleteProgram(m_programID);

    glDeleteShader(m_vertexID);
    glDeleteShader(m_fragmentID);
}

bool GLShaderProgram::LoadShaderFromFile(string path)
{
    std::string shaderString;
    std::ifstream sourceFile(path.c_str());

    if (sourceFile)
    {
        std::ostringstream vertexShader;
        std::ostringstream fragmentShader;

        // Read until start of first shader
        string nextLine;
        getline(sourceFile, nextLine);
        while (!IsShaderTypeDelimiter(nextLine))
        {
            getline(sourceFile, nextLine);
        }

        // Read vertex shader
        getline(sourceFile, nextLine);
        while (!IsShaderTypeDelimiter(nextLine))
        {
            vertexShader << nextLine << "\n";
            getline(sourceFile, nextLine);
        }

        // Read fragment shader
        getline(sourceFile, nextLine);
        while (!sourceFile.eof())
        {
            fragmentShader << nextLine << "\n";
            getline(sourceFile, nextLine);
        }
        fragmentShader << nextLine;

        m_vertexID = CompileShader(vertexShader.str(), path, GL_VERTEX_SHADER);
        m_fragmentID = CompileShader(fragmentShader.str(), path, GL_FRAGMENT_SHADER);
    }
    else
    {
        printf("Unable to open shader file %s\n", path.c_str());
        return false;
    }

    return true;
}

// from http://lazyfoo.net/tutorials/OpenGL/30_loading_text_file_shaders/index.php
GLuint GLShaderProgram::CompileShader(string source, string path, GLenum type)
{
    if (type == GL_VERTEX_SHADER)
    {
        printf("Compiling vertex shader: %s\n", path.c_str());
    }
    else if (type == GL_FRAGMENT_SHADER)
    {
        printf("Compiling fragment shader: %s\n", path.c_str());
    }

    // Create shader ID
    GLuint shaderID = glCreateShader(type);

    // Set shader source
    const GLchar* shaderSource = source.c_str();
    glShaderSource(shaderID, 1, (const GLchar**)&shaderSource, NULL);

    // Compile shader source
    glCompileShader(shaderID);

    // Check compile log
    GLint bufflen;
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &bufflen);
    if (bufflen > 1)
    {
        GLchar* log_string = new char[bufflen + 1];
        glGetShaderInfoLog(shaderID, bufflen, 0, log_string);
        printf("Compile log found for shader %d :\n%s", shaderID, log_string);

        delete log_string;
    }

    // Check shader compile status
    GLint shaderCompiled = GL_FALSE;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
    if (shaderCompiled != GL_TRUE)
    {
        printf("Error compiling shader %d!\n", shaderID);
        glDeleteShader(shaderID);
        return 0;
    }

    printf("Successfully compiled shader.\n");

    return shaderID;
}

void GLShaderProgram::LinkProgram()
{
    m_programID = glCreateProgram();
    glAttachShader(m_programID, m_vertexID);
    glAttachShader(m_programID, m_fragmentID);
    glBindFragDataLocation(m_programID, 0, "outColor");
    glLinkProgram(m_programID);

    // Check whether link was successful, and print errors if applicable
    GLint success;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLchar errorLog[1024] = { 0 };
        glGetProgramInfoLog(m_programID, 1024, NULL, errorLog);
        printf("Error linking shader program: %s\n", errorLog);
    }
}

bool GLShaderProgram::IsShaderTypeDelimiter(string line)
{
    return (line.length() > 3 && line[0] == '/' && line[1] == '/' && line[2] == '-');
}