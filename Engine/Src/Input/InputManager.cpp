#include "Input\InputManager.h"
#include "Input\GamePad.h"
#include "Window\GameWindow.h"

void InputManager::Startup(GameWindow* gameWindow)
{
    m_gameWindow = gameWindow;
    m_enableDebugCameraControls = true;
}

void InputManager::Shutdown()
{
    // TODO clean up gamepads
}

void InputManager::PollEvents(float deltaTime)
{
    // Mouse / keyboard input
    glfwPollEvents();

    // Gamepad input
    unordered_map<unsigned int, GamePad*>::iterator iter = m_gamePads.begin();
    for (; iter != m_gamePads.end(); iter++)
    {
        iter->second->Refresh();
    }

    if (m_enableDebugCameraControls)
    {
        m_debugCameraControls.Update(deltaTime);
    }
}

eKeyState InputManager::GetKey(eKeyValue key)
{
    int glfwKey = DGWDKeyToGLFWKey(key);
    int ret = glfwGetKey(m_gameWindow->GetGLFWWindow(), glfwKey);
    if (ret == GLFW_PRESS)
    {
        return KEY_PRESS;
    }
    return KEY_RELEASE;
}

bool InputManager::GetKeyPressed(eKeyValue key)
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

GamePad* InputManager::GetGamePad(unsigned int id)
{
    if (m_gamePads.count(id) == 0)
        return NULL;

    return m_gamePads[id];
}

bool InputManager::EnableGamePad(GamePad* pad, unsigned int id, bool enable)
{
    if (enable)
    {
        // Add pad to map if it doesn't exist already
        if (m_gamePads.count(id) != 0)
            return false;

        m_gamePads[id] = pad;
    }
    else
    {
        // Clear pad from map, if it exists
        if (m_gamePads.count(id) == 0)
            return false;

        m_gamePads.erase(id);
    }
    return true;
}

int InputManager::DGWDKeyToGLFWKey(eKeyValue dgwdKey)
{
    // The conversion is simple because the eKeyValue enum was defined to match
    // the GLFW values. If this every changes then this function will need to be updated.
    int glfwKey = (int)dgwdKey;
    return glfwKey;
}