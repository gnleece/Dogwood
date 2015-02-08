#pragma once

#include "Input\Gamepad.h"

#include <Windows.h>
#include <Xinput.h>

class XInputGamepad : public GamePad
{
public:
    XInputGamepad(unsigned int id, float deadzone = 0.25f);

    virtual bool    Refresh();

    virtual bool    IsPressed(AbstractGamepadButtons button);
    virtual float   GetAxisValue(AbstractGamepadAxes axis);

private:
    void            Reset();
    void            NormalizeStickAxis(float x, float y, float& xNorm, float& yNorm);
    float           NormalizeTriggerAxis(BYTE value);

    XINPUT_STATE    m_state;

    float           m_deadzone;

    float           m_axisValues[AbstractGamepadAxes::GAMEPAD_NUM_AXES];
};