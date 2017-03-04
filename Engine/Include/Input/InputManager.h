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
    DGWD_KEY_PRESS,
    DGWD_KEY_RELEASE
};

enum eMouseButtonState
{
    DGWD_MOUSE_BUTTON_PRESS,
    DGWD_MOUSE_BUTTON_RELEASE
};

// These are defined to match the key values in GLFW (see glfw3.h)
enum eKeyValue
{
    DGWD_KEY_SPACE           = 32,
    DGWD_KEY_APOSTROPHE      = 39,
    DGWD_KEY_COMMA           = 44,
    DGWD_KEY_MINUS           = 45,
    DGWD_KEY_PERIOD          = 46,
    DGWD_KEY_SLASH           = 47,
    DGWD_KEY_0               = 48,
    DGWD_KEY_1               = 49,
    DGWD_KEY_2               = 50,
    DGWD_KEY_3               = 51,
    DGWD_KEY_4               = 52,
    DGWD_KEY_5               = 53,
    DGWD_KEY_6               = 54,
    DGWD_KEY_7               = 55,
    DGWD_KEY_8               = 56,
    DGWD_KEY_9               = 57,
    DGWD_KEY_SEMICOLON       = 59,
    DGWD_KEY_EQUAL           = 61,
    DGWD_KEY_A               = 65,
    DGWD_KEY_B               = 66,
    DGWD_KEY_C               = 67,
    DGWD_KEY_D               = 68,
    DGWD_KEY_E               = 69,
    DGWD_KEY_F               = 70,
    DGWD_KEY_G               = 71,
    DGWD_KEY_H               = 72,
    DGWD_KEY_I               = 73,
    DGWD_KEY_J               = 74,
    DGWD_KEY_K               = 75,
    DGWD_KEY_L               = 76,
    DGWD_KEY_M               = 77,
    DGWD_KEY_N               = 78,
    DGWD_KEY_O               = 79,
    DGWD_KEY_P               = 80,
    DGWD_KEY_Q               = 81,
    DGWD_KEY_R               = 82,
    DGWD_KEY_S               = 83,
    DGWD_KEY_T               = 84,
    DGWD_KEY_U               = 85,
    DGWD_KEY_V               = 86,
    DGWD_KEY_W               = 87,
    DGWD_KEY_X               = 88,
    DGWD_KEY_Y               = 89,
    DGWD_KEY_Z               = 90,
    DGWD_KEY_LEFT_BRACKET    = 91,
    DGWD_KEY_BACKSLASH       = 92,
    DGWD_KEY_RIGHT_BRACKET   = 93,
    DGWD_KEY_GRAVE_ACCENT    = 96,

    DGWD_KEY_ESCAPE          = 256,
    DGWD_KEY_ENTER           = 257,
    DGWD_KEY_TAB             = 258,
    DGWD_KEY_BACKSPACE       = 259,
    DGWD_KEY_INSERT          = 260,
    DGWD_KEY_DELETE          = 261,
    DGWD_KEY_RIGHT           = 262,
    DGWD_KEY_LEFT            = 263,
    DGWD_KEY_DOWN            = 264,
    DGWD_KEY_UP              = 265,
    DGWD_KEY_PAGE_UP         = 266,
    DGWD_KEY_PAGE_DOWN       = 267,
    DGWD_KEY_HOME            = 268,
    DGWD_KEY_END             = 269,
    DGWD_KEY_CAPS_LOCK       = 280,
    DGWD_KEY_SCROLL_LOCK     = 281,
    DGWD_KEY_NUM_LOCK        = 282,
    DGWD_KEY_PRINT_SCREEN    = 283,
    DGWD_KEY_PAUSE           = 284,
    DGWD_KEY_F1              = 290,
    DGWD_KEY_F2              = 291,
    DGWD_KEY_F3              = 292,
    DGWD_KEY_F4              = 293,
    DGWD_KEY_F5              = 294,
    DGWD_KEY_F6              = 295,
    DGWD_KEY_F7              = 296,
    DGWD_KEY_F8              = 297,
    DGWD_KEY_F9              = 298,
    DGWD_KEY_F10             = 299,
    DGWD_KEY_F11             = 300,
    DGWD_KEY_F12             = 301,
    DGWD_KEY_F13             = 302,
    DGWD_KEY_F14             = 303,
    DGWD_KEY_F15             = 304,
    DGWD_KEY_F16             = 305,
    DGWD_KEY_F17             = 306,
    DGWD_KEY_F18             = 307,
    DGWD_KEY_F19             = 308,
    DGWD_KEY_F20             = 309,
    DGWD_KEY_F21             = 310,
    DGWD_KEY_F22             = 311,
    DGWD_KEY_F23             = 312,
    DGWD_KEY_F24             = 313,
    DGWD_KEY_F25             = 314,
    DGWD_KEY_KP_0            = 320,
    DGWD_KEY_KP_1            = 321,
    DGWD_KEY_KP_2            = 322,
    DGWD_KEY_KP_3            = 323,
    DGWD_KEY_KP_4            = 324,
    DGWD_KEY_KP_5            = 325,
    DGWD_KEY_KP_6            = 326,
    DGWD_KEY_KP_7            = 327,
    DGWD_KEY_KP_8            = 328,
    DGWD_KEY_KP_9            = 329,
    DGWD_KEY_KP_DECIMAL      = 330,
    DGWD_KEY_KP_DIVIDE       = 331,
    DGWD_KEY_KP_MULTIPLY     = 332,
    DGWD_KEY_KP_SUBTRACT     = 333,
    DGWD_KEY_KP_ADD          = 334,
    DGWD_KEY_KP_ENTER        = 335,
    DGWD_KEY_KP_EQUAL        = 336,
    DGWD_KEY_LEFT_SHIFT      = 340,
    DGWD_KEY_LEFT_CONTROL    = 341,
    DGWD_KEY_LEFT_ALT        = 342,
    DGWD_KEY_LEFT_SUPER      = 343,
    DGWD_KEY_RIGHT_SHIFT     = 344,
    DGWD_KEY_RIGHT_CONTROL   = 345,
    DGWD_KEY_RIGHT_ALT       = 346,
    DGWD_KEY_RIGHT_SUPER     = 347,
    DGWD_KEY_MENU            = 348,
};

// These are defined the match the mouse button values in GLFW (see glfw3.h)
enum eMouseButtonValue
{
    DGWD_MOUSE_BUTTON_LEFT      = 0,
    DGWD_MOUSE_BUTTON_RIGHT     = 1,
    DGWD_MOUSE_BUTTON_MIDDLE    = 2
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

    eKeyState           GetKey(eKeyValue key);
    bool                GetKeyPressed(eKeyValue key);
    eMouseButtonState   GetMouseButton(eMouseButtonValue button);
    bool                GetMouseButtonPressed(eMouseButtonValue button);
    CursorPos           GetCursorPos();

    GamePad*            GetGamePad(unsigned int id);
    bool                EnableGamePad(GamePad* pad, unsigned int id, bool enable = true);

    // TODO temp for debugging, remove me
    void                EnableDebugCameraControls(bool enable);

private:
    int                 DGWDKeyToGLFWKey(eKeyValue dgwdKey);
    int                 DGWDMouseButtonToGLFWMouseButton(eMouseButtonValue dgwdButton);

    GameWindow*                             m_gameWindow;
    unordered_map<unsigned int, GamePad*>   m_gamePads;

    bool                                    m_enableDebugCameraControls;
    DebugCameraControls                     m_debugCameraControls;
};