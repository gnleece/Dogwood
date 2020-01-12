#pragma once

#include <utility>
#include <unordered_map>
#include "Debugging\DebugCameraControls.h"
#include "Input/InputManager.h"

class GamePad;
class GameWindow;

using std::pair;
using std::unordered_map;

class InputManagerImpl : public InputManager
{
public:
    static InputManagerImpl* Create();

    virtual void Startup(GameWindow* gameWindow);
    virtual void Shutdown();

    virtual void PollEvents(float deltaTime);

    virtual bool GetKeyPressed(eKeyValue key);
    virtual bool GetMouseButtonPressed(eMouseButtonValue button);
    virtual CursorPos GetCursorPosition();

    virtual GamePad*    GetGamePad(unsigned int id);
    virtual bool        EnableGamePad(GamePad* pad, unsigned int id, bool enable = true);

private:
    GameWindow* m_gameWindow;
    unordered_map<unsigned int, GamePad*>   m_gamePads;

    bool                m_enableDebugCameraControls;
    DebugCameraControls m_debugCameraControls;
};