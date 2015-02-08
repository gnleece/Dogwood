#include "Input\XInputGamepad.h"
#include <limits>
#include <math.h>

WORD AbstractButtonToXInputButton[] =
{
    XINPUT_GAMEPAD_START,           // GAMEPAD_START
    XINPUT_GAMEPAD_BACK,            // GAMEPAD_PAUSE

    XINPUT_GAMEPAD_DPAD_UP,         // GAMEPAD_LPAD_UP
    XINPUT_GAMEPAD_DPAD_DOWN,       // GAMEPAD_LPAD_DOWN
    XINPUT_GAMEPAD_DPAD_LEFT,       // GAMEPAD_LPAD_LEFT
    XINPUT_GAMEPAD_DPAD_RIGHT,      // GAMEPAD_LPAD_RIGHT

    XINPUT_GAMEPAD_Y,               // GAMEPAD_RPAD_UP
    XINPUT_GAMEPAD_A,               // GAMEPAD_RPAD_DOWN
    XINPUT_GAMEPAD_X,               // GAMEPAD_RPAD_LEFT
    XINPUT_GAMEPAD_B,               // GAMEPAD_RPAD_RIGHT

    XINPUT_GAMEPAD_LEFT_THUMB,      // GAMEPAD_LSTICK_BUTTON
    XINPUT_GAMEPAD_RIGHT_THUMB,     // GAMEPAD_RSTICK_BUTTON

    XINPUT_GAMEPAD_LEFT_SHOULDER,   // GAMEPAD_LSHOULDER
    XINPUT_GAMEPAD_RIGHT_SHOULDER,  // GAMEPAD_RSHOULDER
};

XInputGamepad::XInputGamepad(unsigned int id)
: GamePad(id), m_deadzoneX(0.05f), m_deadzoneY(0.02f)
{
    Reset();
}

XInputGamepad::XInputGamepad(unsigned int id, float deadzoneX, float deadzoneY)
: GamePad(id), m_deadzoneX(deadzoneX), m_deadzoneY(deadzoneY)
{
    Reset();
}

bool XInputGamepad::Refresh()
{
    DWORD dwResult;
    ZeroMemory(&m_state, sizeof(XINPUT_STATE));

    // Get controller state from XInput
    dwResult = XInputGetState(m_id, &m_state);

    if (dwResult == ERROR_SUCCESS)
    {
        m_axisValues[GAMEPAD_LSTICK_X] = NormalizeStickAxis(m_state.Gamepad.sThumbLX, m_deadzoneX);
        m_axisValues[GAMEPAD_LSTICK_Y] = NormalizeStickAxis(m_state.Gamepad.sThumbLX, m_deadzoneX);
        m_axisValues[GAMEPAD_RSTICK_X] = NormalizeStickAxis(m_state.Gamepad.sThumbLX, m_deadzoneX);
        m_axisValues[GAMEAPD_RSTICK_Y] = NormalizeStickAxis(m_state.Gamepad.sThumbLX, m_deadzoneX);

        m_axisValues[GAMEPAD_LTRIGGER] = NormalizeTriggerAxis(m_state.Gamepad.bLeftTrigger);
        m_axisValues[GAMEPAD_RTRIGGER] = NormalizeTriggerAxis(m_state.Gamepad.bRightTrigger);

        m_connected = true;
        return true;
    }

    Reset();
    return false;
}

bool XInputGamepad::IsPressed(AbstractGamepadButtons button)
{
    WORD XInputButton = AbstractButtonToXInputButton[button];
    return (m_state.Gamepad.wButtons & XInputButton) != 0;
}

float XInputGamepad::GetAxisValue(AbstractGamepadAxes axis)
{
    if (axis < 0 || axis >= GAMEPAD_NUM_AXES)
    {
        return 0.f;
    }
    return m_axisValues[axis];
}

void XInputGamepad::Reset()
{
    for (int i = 0; i < AbstractGamepadAxes::GAMEPAD_NUM_AXES; i++)
    {
        m_axisValues[i] = 0.f;
    }
    ZeroMemory(&m_state, sizeof(XINPUT_STATE));

    m_connected = false;
}

float XInputGamepad::NormalizeStickAxis(SHORT value, float deadzone)
{
    // TODO fix me (deadzone calculation is wrong)
    float norm = fmaxf(-1, (float)value / SHRT_MAX);
    float normalized = (abs(norm) < deadzone ? 0 : (abs(norm) - deadzone) * (norm / abs(norm)));

    if (deadzone > 0)
    {
        normalized *= 1 / (1 - m_deadzoneX);
    }

    return normalized;
}

float XInputGamepad::NormalizeTriggerAxis(BYTE value)
{
    return (float)value / UCHAR_MAX;
}