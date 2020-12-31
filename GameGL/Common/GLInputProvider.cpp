#include "GLInputProvider.h"
#include "GLGameWindow.h"
#include "Input/GamePad.h"

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <unordered_map>

GLInputProvider::GLInputProvider(GLGameWindow* gameWindow)
{
    m_gameWindow = gameWindow;
}

void GLInputProvider::PollEvents(float deltaTime)
{
    // Mouse / keyboard input
    glfwPollEvents();

    // Gamepad input
    unordered_map<unsigned int, GamePad*>::iterator iter = m_gamePads.begin();
    for (; iter != m_gamePads.end(); iter++)
    {
        iter->second->Refresh();
    }
}

bool GLInputProvider::GetKeyPressed(eKeyValue key)
{
    return m_gameWindow->GetKeyPressed(key);
}

bool GLInputProvider::GetMouseButtonPressed(eMouseButtonValue button)
{
    return m_gameWindow->GetMouseButtonPressed(button);
}

CursorPos GLInputProvider::GetCursorPosition()
{
    return m_gameWindow->GetCursorPosition();
}

GamePad* GLInputProvider::GetGamePad(unsigned int id)
{
    if (m_gamePads.count(id) == 0)
        return NULL;

    return m_gamePads[id];
}

bool GLInputProvider::EnableGamePad(GamePad* pad, unsigned int id, bool enable)
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