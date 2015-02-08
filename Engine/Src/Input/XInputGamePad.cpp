#include "Input\XInputGamepad.h"
#include <limits>
#include <math.h>


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
    // TODO fix me (map from Xinput buttons to AbstractGamepadButtons)
    //return (m_state.Gamepad.wButtons & /*button*/) != 0;
    return false;
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