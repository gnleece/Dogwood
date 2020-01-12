#include "Input\InputManager.h"
#include "Input\GamePad.h"
#include "Input/OpenGL/InputManagerImpl.h"
#include "Window\GameWindow.h"

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

InputManager* InputManager::Create()
{
    return InputManagerImpl::Create();
}

void InputManager::Destroy(InputManager* inputManager)
{
    delete inputManager;
}

InputManagerImpl* InputManagerImpl::Create()
{
    return new InputManagerImpl();
}

void InputManagerImpl::Startup(GameWindow* gameWindow)
{
    m_gameWindow = gameWindow;
    m_enableDebugCameraControls = true;
}

void InputManagerImpl::Shutdown()
{
    // TODO clean up gamepads
}

void InputManagerImpl::PollEvents(float deltaTime)
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

GamePad* InputManagerImpl::GetGamePad(unsigned int id)
{
    if (m_gamePads.count(id) == 0)
        return NULL;

    return m_gamePads[id];
}

bool InputManagerImpl::EnableGamePad(GamePad* pad, unsigned int id, bool enable)
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

bool InputManagerImpl::GetKeyPressed(eKeyValue key)
{
    return m_gameWindow->GetKeyPressed(key);
}

bool InputManagerImpl::GetMouseButtonPressed(eMouseButtonValue button)
{
    return m_gameWindow->GetMouseButtonPressed(button);
}

CursorPos InputManagerImpl::GetCursorPosition()
{
    return m_gameWindow->GetCursorPosition();
}