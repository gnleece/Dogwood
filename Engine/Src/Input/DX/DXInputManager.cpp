#ifdef GRAPHICS_DX

#include "Input\InputManager.h"
#include "Input\DX\DXInputManager.h"


InputManager* InputManager::Create()
{
    return DXInputManager::Create();
}

void InputManager::Destroy(InputManager* inputManager)
{
    delete inputManager;
}

DXInputManager* DXInputManager::Create()
{
    return new DXInputManager();
}

void DXInputManager::Startup(GameWindow* gameWindow)
{

}

void DXInputManager::Shutdown()
{

}

void DXInputManager::PollEvents(float deltaTime)
{

}

GamePad* DXInputManager::GetGamePad(unsigned int id)
{
    return NULL;
}

bool DXInputManager::EnableGamePad(GamePad* pad, unsigned int id, bool enable)
{
    return false;
}

bool DXInputManager::GetKeyPressed(eKeyValue key)
{
    return false;
}

bool DXInputManager::GetMouseButtonPressed(eMouseButtonValue button)
{
    return false;
}

CursorPos DXInputManager::GetCursorPosition()
{
    return CursorPos(0, 0);
}

#endif