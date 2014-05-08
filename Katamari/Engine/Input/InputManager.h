#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

class InputManager
{
public:
    static InputManager& Singleton()
    {
        static InputManager    singleton;
        return singleton;
    }
    InputManager() {}

    void        Startup(GLFWwindow* gameWindow);
    void        Shutdown();
};