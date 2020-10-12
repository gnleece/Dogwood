#ifdef GRAPHICS_GL

#include "Input\InputManager.h"
#include "Input\GamePad.h"
#include "Input/GL/GLInputManager.h"
#include "Window\GameWindow.h"

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

InputManager* InputManager::Create()
{
    return GLInputManager::Create();
}

void InputManager::Destroy(InputManager* inputManager)
{
    delete inputManager;
}

GLInputManager* GLInputManager::Create()
{
    return new GLInputManager();
}

void GLInputManager::Startup(GameWindow* gameWindow)
{
    m_gameWindow = gameWindow;
    m_enableDebugCameraControls = true;
}

void GLInputManager::Shutdown()
{
    // TODO clean up gamepads
}

void GLInputManager::PollEvents(float deltaTime)
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

GamePad* GLInputManager::GetGamePad(unsigned int id)
{
    if (m_gamePads.count(id) == 0)
        return NULL;

    return m_gamePads[id];
}

bool GLInputManager::EnableGamePad(GamePad* pad, unsigned int id, bool enable)
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

bool GLInputManager::GetKeyPressed(eKeyValue key)
{
    return m_gameWindow->GetKeyPressed(key);
}

bool GLInputManager::GetMouseButtonPressed(eMouseButtonValue button)
{
    return m_gameWindow->GetMouseButtonPressed(button);
}

CursorPos GLInputManager::GetCursorPosition()
{
    return m_gameWindow->GetCursorPosition();
}

#endif