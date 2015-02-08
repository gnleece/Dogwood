#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include <utility>
#include <unordered_map>
#include "..\Debugging\DebugCameraControls.h"

class GamePad;
class GameWindow;

using std::pair;
using std::unordered_map;
typedef pair<float, float> CursorPos;

enum eKeyState
{
    KEY_PRESS,
    KEY_RELEASE
};

enum eMouseButtonState
{
    MOUSE_BUTTON_PRESS,
    MOUSE_BUTTON_RELEASE
};

class InputManager
{
public:

    static InputManager& Singleton()
    {
        static InputManager    singleton;
        return singleton;
    }
    InputManager() {}

    void                Startup(GameWindow* gameWindow);
    void                Shutdown();

    void                PollEvents(float deltaTime);

    // TODO: define own key enum so client code doesn't touch GLFW
    eKeyState           GetKey(int key);
    bool                GetKeyPressed(int key);
    eMouseButtonState   GetMouseButton(int button);
    bool                GetMouseButtonPressed(int button);
    CursorPos           GetCursorPos();

    GamePad*            GetGamePad(unsigned int id);
    bool                EnableGamePad(GamePad* pad, unsigned int id, bool enable = true);

    // TODO temp for debugging, remove me
    void                EnableDebugCameraControls(bool enable);

private:
    GameWindow*                             m_gameWindow;
    unordered_map<unsigned int, GamePad*>   m_gamePads;

    bool                                    m_enableDebugCameraControls;
    DebugCameraControls                     m_debugCameraControls;
};