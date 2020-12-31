#pragma once
#include <Input\Input.h>

class GamePad;

class InputProvider
{
public:
    virtual void        PollEvents(float deltaTime) = 0;

    virtual bool        GetKeyPressed(eKeyValue key) = 0;
    virtual bool        GetMouseButtonPressed(eMouseButtonValue button) = 0;
    virtual CursorPos   GetCursorPosition() = 0;

    virtual GamePad*    GetGamePad(unsigned int id) = 0;
    virtual bool        EnableGamePad(GamePad* pad, unsigned int id, bool enable = true) = 0;
};