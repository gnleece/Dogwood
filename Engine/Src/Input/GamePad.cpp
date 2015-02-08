#include "Input\Gamepad.h"

GamePad::GamePad(unsigned int id)
 : m_id(id), m_connected(false)
{
}

bool GamePad::Refresh()
{
    return false;
}

bool GamePad::Connected()
{
    return m_connected;
}

bool GamePad::IsPressed(AbstractGamepadButtons)
{
    return false;
}

float GamePad::GetAxisValue(AbstractGamepadAxes)
{
    return 0.f;
}