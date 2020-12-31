#include "Input/InputManager.h"
#include "Input/InputProvider.h"

void InputManager::Startup(InputProvider* inputProvider)
{
    m_inputProvider = inputProvider;
}

void InputManager::Shutdown()
{

}

void InputManager::PollEvents(float deltaTime)
{
    m_inputProvider->PollEvents(deltaTime);
}

bool InputManager::GetKeyPressed(eKeyValue key)
{
    return m_inputProvider->GetKeyPressed(key);
}

bool InputManager::GetMouseButtonPressed(eMouseButtonValue button)
{
    return m_inputProvider->GetMouseButtonPressed(button);
}

CursorPos InputManager::GetCursorPosition()
{
    return m_inputProvider->GetCursorPosition();
}

GamePad* InputManager::GetGamePad(unsigned int id)
{
    return m_inputProvider->GetGamePad(id);
}

bool InputManager::EnableGamePad(GamePad* pad, unsigned int id, bool enable)
{
    return m_inputProvider->EnableGamePad(pad, id, enable);
}