#pragma once

#include <utility>
#include <unordered_map>
#include "Debugging\DebugCameraControls.h"
#include "Input/Input.h"

class GamePad;
class GameWindow;

using std::pair;
using std::unordered_map;

class InputManager
{
public:

    static InputManager& Singleton()
    {
        static InputManager singleton;
        return singleton;
    }
    InputManager() {}

    void Startup(GameWindow* gameWindow);
    void Shutdown();

    void PollEvents(float deltaTime);

    bool        GetKeyPressed(eKeyValue key);
    bool        GetMouseButtonPressed(eMouseButtonValue button);
    CursorPos   GetCursorPosition();

    GamePad*    GetGamePad(unsigned int id);
    bool        EnableGamePad(GamePad* pad, unsigned int id, bool enable = true);

private:
    GameWindow* m_gameWindow;
};