#pragma once

#include <string>
#include "Input/Input.h"

using std::string;

class GamePad;

class GameWindow
{
public:
    virtual void        PollEvents(float deltaTime) = 0;

    virtual void        SwapBuffers() = 0;
    virtual bool        ShouldClose() = 0;

    virtual int         GetWidth() = 0;
    virtual int         GetHeight() = 0;

    virtual float       GetLastFrameTime() = 0;

    virtual bool        GetKeyPressed(eKeyValue key) = 0;
    virtual bool        GetMouseButtonPressed(eMouseButtonValue button) = 0;
    virtual CursorPos   GetCursorPosition() = 0;

    virtual GamePad*    GetGamePad(unsigned int id) = 0;
    virtual bool        EnableGamePad(GamePad* pad, unsigned int id, bool enable = true) = 0;
};