#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(std::string vertexShaderPath, std::string fragmentShaderPath)
{
    Load(vertexShaderPath, fragmentShaderPath);
}

void ShaderProgram::Load(std::string vertexShaderPath, std::string fragmentShaderPath)
{
    //TODO check if already loaded and delete
    m_vertexID = LoadShaderFromFile(vertexShaderPath, GL_VERTEX_SHADER);
    m_fragmentID = LoadShaderFromFile(fragmentShaderPath, GL_FRAGMENT_SHADER);

    m_programID = LinkProgram(m_vertexID, m_fragmentID);

    m_attribLocations[ATTRIB_POS]       = glGetAttribLocation(m_programID, "position");
    m_attribLocations[ATTRIB_NORMAL]    = glGetAttribLocation(m_programID, "normal");
    m_attribLocations[ATTRIB_TEXCOORD]  = glGetAttribLocation(m_programID, "texcoord");
}

GLint ShaderProgram::GetAttribLocation(eAttribType attrib) const
{
    if (attrib < NUM_ATTRIBS)
    {
        return m_attribLocations[attrib];
    }
    return 0;
}

void ShaderProgram::Delete()
{
    glDeleteProgram(m_programID);

    glDeleteShader(m_vertexID);
    glDeleteShader(m_fragmentID);
}

// from http://lazyfoo.net/tutorials/OpenGL/30_loading_text_file_shaders/index.php
GLuint ShaderProgram::LoadShaderFromFile(std::string path, GLenum shaderType)
{
    // Open file
    GLuint shaderID = 0;
    std::string shaderString;
    std::ifstream sourceFile(path.c_str());

    // Source file loaded
    if (sourceFile)
    {
        // Get shader source
        shaderString.assign((std::istreambuf_iterator< char >(sourceFile)), std::istreambuf_iterator< char >());
        // Create shader ID
        shaderID = glCreateShader(shaderType);

        // Set shader source
        const GLchar* shaderSource = shaderString.c_str();
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
            printf("Compile Log found for shader %d :\n%s", shaderID, log_string);

            delete log_string;
        }

        // Check shader compile status
        GLint shaderCompiled = GL_FALSE;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
        if (shaderCompiled != GL_TRUE)
        {
            printf("Unable to compile shader %d!\n", shaderID);
            glDeleteShader(shaderID);
            return 0;
        }

        printf("Shader load success!\n\n");
    }
    else
    {
        printf("Unable to open file %s\n", path.c_str());
    }

    return shaderID;
}

GLuint ShaderProgram::LinkProgram(GLuint vertexShader, GLuint fragmentShader)
{
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColour");
    glLinkProgram(shaderProgram);
    return shaderProgram;
}