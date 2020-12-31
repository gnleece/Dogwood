#pragma once

#include "Window/GameWindow.h"

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include <string>

using std::string;

class GLGameWindow : public GameWindow
{
public:
    GLGameWindow(string name, int width, int height);
    void Destroy();

    void SwapBuffers();
    bool ShouldClose();

    int GetWidth();
    int GetHeight();

    bool GetKeyPressed(eKeyValue key);
    bool GetMouseButtonPressed(eMouseButtonValue button);
    CursorPos GetCursorPosition();

    virtual float GetLastFrameTime();

private:
    static void ErrorCallback(int error, const char* description);

    static int DGWDKeyToGLFWKey(eKeyValue dgwdKey);
    static int DGWDMouseButtonToGLFWMouseButton(eMouseButtonValue dgwdButton);

    GLFWwindow* m_window;

    string      m_name;
    int         m_width;
    int         m_height;
};