#include "Window/GameWindow.h"
#include "Window/GL/GLGameWindow.h"

#include "Math/Algebra.h"

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

GameWindow* GameWindow::Create()
{
    return GLGameWindow::Create();
}

void GameWindow::Destroy(GameWindow* gameWindow)
{
    delete gameWindow;
}

GLGameWindow* GLGameWindow::Create()
{
    return new GLGameWindow();
}

void GLGameWindow::Setup(string name, int width, int height)
{
    m_name = name;
    m_width = width;
    m_height = height;

    glfwSetErrorCallback(ErrorCallback);
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    m_window = glfwCreateWindow(m_width, m_height, m_name.c_str(), NULL, NULL);
    if (!m_window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_window);
}

void GLGameWindow::Destroy()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void GLGameWindow::SwapBuffers()
{
    glfwSwapBuffers(m_window);
}

bool GLGameWindow::ShouldClose()
{
    return glfwWindowShouldClose(m_window) != 0;
}

int GLGameWindow::GetWidth()
{
    return m_width;
}

int GLGameWindow::GetHeight()
{
    return m_height;
}

bool GLGameWindow::GetKeyPressed(eKeyValue key)
{
    int glfwKey = DGWDKeyToGLFWKey(key);
    int ret = glfwGetKey(m_window, glfwKey);
    return ret == GLFW_PRESS;
}

bool GLGameWindow::GetMouseButtonPressed(eMouseButtonValue button)
{
    int glfwButton = DGWDMouseButtonToGLFWMouseButton(button);
    int ret = glfwGetMouseButton(m_window, glfwButton);
    return ret == GLFW_PRESS;
}

CursorPos GLGameWindow::GetCursorPosition()
{
    double xPos, yPos;
    glfwGetCursorPos(m_window, &xPos, &yPos);
    return CursorPos((float)xPos, (float)yPos);
}

float GLGameWindow::GetLastFrameTime()
{
    return (float)glfwGetTime();
}

void GLGameWindow::ErrorCallback(int error, const char* description)
{
    fputs(description, stderr);
}

int GLGameWindow::DGWDKeyToGLFWKey(eKeyValue dgwdKey)
{
    // The conversion is simple because the eKeyValue enum was defined to match
    // the GLFW values. If this ever changes then this function will need to be updated.
    int glfwKey = (int)dgwdKey;
    return glfwKey;
}

int GLGameWindow::DGWDMouseButtonToGLFWMouseButton(eMouseButtonValue dgwdButton)
{
    // The conversion is simple because the eButtonValue enum was defined to match
    // the GLFW values. If this ever changes then this function will need to be updated.
    int glfwButton = (int)dgwdButton;
    return glfwButton;
}