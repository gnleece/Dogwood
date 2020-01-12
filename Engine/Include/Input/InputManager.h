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

    static InputManager* Singleton()
    {
        static InputManager* singleton = InputManager::Create();
        return singleton;
    }
    InputManager() {}

    virtual void Startup(GameWindow* gameWindow) = 0;
    virtual void Shutdown() = 0;

    virtual void PollEvents(float deltaTime) = 0;

    virtual bool        GetKeyPressed(eKeyValue key) = 0;
    virtual bool        GetMouseButtonPressed(eMouseButtonValue button) = 0;
    virtual CursorPos   GetCursorPosition() = 0;

    virtual GamePad*    GetGamePad(unsigned int id) = 0;
    virtual bool        EnableGamePad(GamePad* pad, unsigned int id, bool enable = true) = 0;

private:
    static InputManager*    Create();
    static void             Destroy(InputManager* inputManager);
};