#pragma once

enum AbstractGamepadButtons
{
    // Start, pause
    GAMEPAD_START,
    GAMEPAD_PAUSE,

    // Left pad (D-pad)
    GAMEPAD_LPAD_UP,
    GAMEPAD_LPAD_DOWN,
    GAMEPAD_LPAD_LEFT,
    GAMEPAD_LPAD_RIGHT,

    // Right pad (buttons)
    GAMEPAD_RPAD_UP,
    GAMEPAD_RPAD_DOWN,
    GAMEPAD_RPAD_LEFT,
    GAMEPAD_RPAD_RIGHT,

    // Stick buttons
    GAMEPAD_LSTICK_BUTTON,
    GAMEPAD_RSTICK_BUTTON,

    // Shoulder buttons
    GAMEPAD_LSHOULDER,
    GAMEPAD_RSHOULDER,

    GAMEPAD_NUM_BUTTONS
};

enum AbstractGamepadAxes
{
    // Stick axes
    GAMEPAD_LSTICK_X,
    GAMEPAD_LSTICK_Y,
    GAMEPAD_RSTICK_X,
    GAMEPAD_RSTICK_Y,

    // Trigger axes
    GAMEPAD_LTRIGGER,
    GAMEPAD_RTRIGGER,

    GAMEPAD_NUM_AXES
};

class GamePad
{
public:
    GamePad(unsigned int id);

    virtual bool    Refresh();
    virtual bool    Connected();

    virtual bool    IsPressed(AbstractGamepadButtons);
    virtual float   GetAxisValue(AbstractGamepadAxes);

protected:
    unsigned int    m_id;
    bool            m_connected;
};