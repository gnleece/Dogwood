#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include <string>
#include "Rendering\ShaderProgram.h"

class Game
{
public:
    Game(std::string name, int windowWidth, int windowHeight);
    void Run();
    void Shutdown();

private:
    void WindowSetup();
    void RenderingSetup();
    void WindowCleanup();

    std::string     m_name;

    GLFWwindow*     m_window;
    int             m_windowWidth;
    int             m_windowHeight;

    ShaderProgram   m_shaderProgram;
};