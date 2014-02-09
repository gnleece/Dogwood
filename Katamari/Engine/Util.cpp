#include "Util.h"

// from http://lazyfoo.net/tutorials/OpenGL/30_loading_text_file_shaders/index.php
GLuint LoadShaderFromFile( std::string path, GLenum shaderType )
{
    //Open file
    GLuint shaderID = 0;
    std::string shaderString;
    std::ifstream sourceFile( path.c_str() );

    //Source file loaded
    if( sourceFile )
    {
        //Get shader source
        shaderString.assign( ( std::istreambuf_iterator< char >( sourceFile ) ), std::istreambuf_iterator< char >() );
        //Create shader ID
        shaderID = glCreateShader( shaderType );

        //Set shader source
        const GLchar* shaderSource = shaderString.c_str();
        glShaderSource( shaderID, 1, (const GLchar**)&shaderSource, NULL );

        //Compile shader source
        glCompileShader( shaderID );

        //Check shader for errors
        GLint shaderCompiled = GL_FALSE;
        glGetShaderiv( shaderID, GL_COMPILE_STATUS, &shaderCompiled );
        if( shaderCompiled != GL_TRUE )
        {
            printf( "Unable to compile shader %d!\n\nSource:\n%s\n", shaderID, shaderSource );
            glDeleteShader( shaderID );
            shaderID = 0;
        }
        printf("Shader load success!\n");
    }
    else
    {
        printf( "Unable to open file %s\n", path.c_str() );
    }

    return shaderID;
}

GLuint LinkShaderProgram(GLuint vertexShader, GLuint fragmentShader)
{
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColour");
    glLinkProgram(shaderProgram);
    printf("foo bar baz\n");
    return shaderProgram;
}