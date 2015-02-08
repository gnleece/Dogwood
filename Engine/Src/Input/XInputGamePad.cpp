#include "Input\XInputGamepad.h"
#include "Math\Algebra.h"
#include <limits>
#include <math.h>

#include <stdio.h>

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

XInputGamepad::XInputGamepad(unsigned int id, float deadzone)
: GamePad(id), m_deadzone(deadzone)
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
        float xNorm, yNorm;

        NormalizeStickAxis(m_state.Gamepad.sThumbLX, m_state.Gamepad.sThumbLY, xNorm, yNorm);
        m_axisValues[GAMEPAD_LSTICK_X] = xNorm;
        m_axisValues[GAMEPAD_LSTICK_Y] = yNorm;

        NormalizeStickAxis(m_state.Gamepad.sThumbRX, m_state.Gamepad.sThumbRY, xNorm, yNorm);
        m_axisValues[GAMEPAD_RSTICK_X] = xNorm;
        m_axisValues[GAMEPAD_RSTICK_Y] = yNorm;

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


void XInputGamepad::NormalizeStickAxis(float x, float y, float& xNorm, float& yNorm)
{
    // Dead-zone example code based on https://msdn.microsoft.com/en-us/library/windows/desktop/ee417001

    xNorm = x;
    yNorm = y;

    // Determine how far the controller is pushed
    float magnitude = sqrt(x*x + y*y) / SHRT_MAX;

    // Check if the controller is outside a circular dead zone
    if (magnitude > m_deadzone)
    {
        // Clip the magnitude at its expected min/max values
        xNorm = Clamp(xNorm, SHRT_MIN, SHRT_MAX);
        yNorm = Clamp(yNorm, SHRT_MIN, SHRT_MAX);

        // Adjust magnitude relative to the end of the dead zone
        xNorm += m_deadzone * (xNorm > 0 ? -1 : 1);
        yNorm += m_deadzone * (yNorm > 0 ? -1 : 1);

        // Normalize to [-1, 1] range
        xNorm = Clamp(xNorm / (SHRT_MAX - m_deadzone), -1, 1);
        yNorm = Clamp(yNorm / (SHRT_MAX - m_deadzone), -1, 1);
    }
    else // If the controller is in the deadzone zero out the magnitude
    {
        xNorm = 0.0f;
        yNorm = 0.0f;
    }
}

float XInputGamepad::NormalizeTriggerAxis(BYTE value)
{
    return (float)value / UCHAR_MAX;
}