#include "Input\InputManager.h"
#include "Window\GameWindow.h"

void InputManager::Startup(GameWindow* gameWindow)
{
    m_gameWindow = gameWindow;
    m_enableDebugCameraControls = true;
}

void InputManager::Shutdown()
{}

void InputManager::PollEvents(float deltaTime)
{
    glfwPollEvents();

    if (m_enableDebugCameraControls)
    {
        m_debugCameraControls.Update(deltaTime);
    }
}

eKeyState InputManager::GetKey(int key)
{
    int ret = glfwGetKey(m_gameWindow->GetGLFWWindow(), key);
    if (ret == GLFW_PRESS)
    {
        return KEY_PRESS;
    }
    return KEY_RELEASE;
}

bool InputManager::GetKeyPressed(int key)
{
    return GetKey(key) == KEY_PRESS;
}

eMouseButtonState InputManager::GetMouseButton(int button)
{
    int ret = glfwGetMouseButton(m_gameWindow->GetGLFWWindow(), button);
    if (ret == GLFW_PRESS)
    {
        return MOUSE_BUTTON_PRESS;
    }
    return MOUSE_BUTTON_RELEASE;
}

bool InputManager::GetMouseButtonPressed(int button)
{
    return GetMouseButton(button) == MOUSE_BUTTON_PRESS;
}

CursorPos InputManager::GetCursorPos()
{
    double xPos, yPos;
    glfwGetCursorPos(m_gameWindow->GetGLFWWindow(), &xPos, &yPos);
    return CursorPos((float)xPos, (float)yPos);
}