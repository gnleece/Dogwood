#include "Input/InputManager.h"
#include "Window/GameWindow.h"

void InputManager::Startup(GameWindow* gameWindow)
{
    m_gameWindow = gameWindow;
}

void InputManager::Shutdown()
{

}

void InputManager::PollEvents(float deltaTime)
{
    m_gameWindow->PollEvents(deltaTime);
}

bool InputManager::GetKeyPressed(eKeyValue key)
{
    return m_gameWindow->GetKeyPressed(key);
}

bool InputManager::GetMouseButtonPressed(eMouseButtonValue button)
{
    return m_gameWindow->GetMouseButtonPressed(button);
}

CursorPos InputManager::GetCursorPosition()
{
    return m_gameWindow->GetCursorPosition();
}

GamePad* InputManager::GetGamePad(unsigned int id)
{
    return m_gameWindow->GetGamePad(id);
}

bool InputManager::EnableGamePad(GamePad* pad, unsigned int id, bool enable)
{
    return m_gameWindow->EnableGamePad(pad, id, enable);
}