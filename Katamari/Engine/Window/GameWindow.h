#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include <string>

using std::string;

class GameWindow
{
public:
    void Setup(string name, int width, int height);
    void Destroy();

    void SwapBuffers();
    bool ShouldClose();

    void SetKeyCallback(GLFWkeyfun callback);
    void SetMouseButtonCallback(GLFWmousebuttonfun callback);
    void SetCursorPositionCallback(GLFWcursorposfun callback);

private:
    static void ErrorCallback(int error, const char* description);

    GLFWwindow* m_window;

    string      m_name;
    int         m_width;
    int         m_height;
};