#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include <utility>
#include "..\Debugging\DebugCameraControls.h"

class GameWindow;

using std::pair;
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

    void Startup(GameWindow* gameWindow);
    void Shutdown();

    void PollEvents();

    // TODO: define own key enum so client code doesn't touch GLFW
    eKeyState           GetKey(int key);
    bool                GetKeyPressed(int key);
    eMouseButtonState   GetMouseButton(int button);
    bool                GetMouseButtonPressed(int button);
    CursorPos           GetCursorPos();

    void EnableDebugCameraControls(bool enable);

private:
    GameWindow* m_gameWindow;

    bool                m_enableDebugCameraControls;
    DebugCameraControls m_debugCameraControls;
};