#pragma once

#include "Input\Gamepad.h"

#include <Windows.h>
#include <Xinput.h>

class XInputGamepad : public GamePad
{
public:
    XInputGamepad(unsigned int id);
    XInputGamepad(unsigned int id, float deadzoneX, float deadzoneY);

    virtual bool    Refresh();

    virtual bool    IsPressed(AbstractGamepadButtons button);
    virtual float   GetAxisValue(AbstractGamepadAxes axis);

private:
    void            Reset();
    float           NormalizeStickAxis(SHORT value, float deadzone);
    float           NormalizeTriggerAxis(BYTE value);

    XINPUT_STATE    m_state;

    float           m_deadzoneX;
    float           m_deadzoneY;

    float           m_axisValues[AbstractGamepadAxes::GAMEPAD_NUM_AXES];
};