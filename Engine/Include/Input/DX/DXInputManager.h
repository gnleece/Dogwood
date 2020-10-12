#pragma once

#ifdef GRAPHICS_DX

#include "Input/InputManager.h"

class GamePad;
class GameWindow;

using std::pair;
using std::unordered_map;

class DXInputManager : public InputManager
{
public:
    static DXInputManager* Create();

    virtual void Startup(GameWindow* gameWindow);
    virtual void Shutdown();

    virtual void PollEvents(float deltaTime);

    virtual bool GetKeyPressed(eKeyValue key);
    virtual bool GetMouseButtonPressed(eMouseButtonValue button);
    virtual CursorPos GetCursorPosition();

    virtual GamePad*    GetGamePad(unsigned int id);
    virtual bool        EnableGamePad(GamePad* pad, unsigned int id, bool enable = true);
};

#endif