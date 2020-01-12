#pragma once

#include "Window/GameWindow.h"

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include <string>

using std::string;

class GameWindowImpl : public GameWindow
{
public:
    static GameWindowImpl* Create();

    virtual void Setup(string name, int width, int height);
    virtual void Destroy();

    virtual void SwapBuffers();
    virtual bool ShouldClose();

    virtual int GetWidth();
    virtual int GetHeight();

    virtual bool GetKeyPressed(eKeyValue key);
    virtual bool GetMouseButtonPressed(eMouseButtonValue button);
    virtual CursorPos GetCursorPosition();

private:
    static void ErrorCallback(int error, const char* description);

    static int DGWDKeyToGLFWKey(eKeyValue dgwdKey);
    static int DGWDMouseButtonToGLFWMouseButton(eMouseButtonValue dgwdButton);

    GLFWwindow* m_window;

    string      m_name;
    int         m_width;
    int         m_height;
};