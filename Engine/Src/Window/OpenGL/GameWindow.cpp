#include "Window\GameWindow.h"
#include "Window/OpenGL/GameWindowImpl.h"

#include "Math\Algebra.h"

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

GameWindow* GameWindow::Create()
{
    return GameWindowImpl::Create();
}

void GameWindow::Destroy(GameWindow* gameWindow)
{
    delete gameWindow;
}

GameWindowImpl* GameWindowImpl::Create()
{
    return new GameWindowImpl();
}

void GameWindowImpl::Setup(string name, int width, int height)
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

void GameWindowImpl::Destroy()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void GameWindowImpl::SwapBuffers()
{
    glfwSwapBuffers(m_window);
}

bool GameWindowImpl::ShouldClose()
{
    return glfwWindowShouldClose(m_window) != 0;
}

int GameWindowImpl::GetWidth()
{
    return m_width;
}

int GameWindowImpl::GetHeight()
{
    return m_height;
}

bool GameWindowImpl::GetKeyPressed(eKeyValue key)
{
    int glfwKey = DGWDKeyToGLFWKey(key);
    int ret = glfwGetKey(m_window, glfwKey);
    return ret == GLFW_PRESS;
}

bool GameWindowImpl::GetMouseButtonPressed(eMouseButtonValue button)
{
    int glfwButton = DGWDMouseButtonToGLFWMouseButton(button);
    int ret = glfwGetMouseButton(m_window, glfwButton);
    return ret == GLFW_PRESS;
}

CursorPos GameWindowImpl::GetCursorPosition()
{
    double xPos, yPos;
    glfwGetCursorPos(m_window, &xPos, &yPos);
    return CursorPos((float)xPos, (float)yPos);
}

void GameWindowImpl::ErrorCallback(int error, const char* description)
{
    fputs(description, stderr);
}

int GameWindowImpl::DGWDKeyToGLFWKey(eKeyValue dgwdKey)
{
    // The conversion is simple because the eKeyValue enum was defined to match
    // the GLFW values. If this ever changes then this function will need to be updated.
    int glfwKey = (int)dgwdKey;
    return glfwKey;
}

int GameWindowImpl::DGWDMouseButtonToGLFWMouseButton(eMouseButtonValue dgwdButton)
{
    // The conversion is simple because the eButtonValue enum was defined to match
    // the GLFW values. If this ever changes then this function will need to be updated.
    int glfwButton = (int)dgwdButton;
    return glfwButton;
}