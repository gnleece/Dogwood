#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

class GameWindow;

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

    void EnableDebugCameraControls(bool enable);

private:
    GameWindow* m_gameWindow;
    bool        m_debugCameraControls;
};