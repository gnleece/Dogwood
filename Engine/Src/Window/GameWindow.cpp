#include "Window\GameWindow.h"

#include "Math\Algebra.h"

void GameWindow::Setup(string name, int width, int height)
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

void GameWindow::Destroy()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void GameWindow::SwapBuffers()
{
    glfwSwapBuffers(m_window);
}

bool GameWindow::ShouldClose()
{
    return glfwWindowShouldClose(m_window) != 0;
}

int GameWindow::GetWidth()
{
    return m_width;
}

int GameWindow::GetHeight()
{
    return m_height;
}

GLFWwindow* GameWindow::GetGLFWWindow()
{
    return m_window;
}

void GameWindow::ErrorCallback(int error, const char* description)
{
    fputs(description, stderr);
}