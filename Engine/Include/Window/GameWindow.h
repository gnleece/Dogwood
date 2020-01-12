#pragma once

#include <string>
#include "Input/Input.h"

using std::string;

class GameWindow
{
public:
    static GameWindow* Create();
    static void Destroy(GameWindow* gameWindow);

    virtual ~GameWindow() {}

    virtual void Setup(string name, int width, int height) = 0;
    virtual void Destroy() = 0;

    virtual void SwapBuffers() = 0;
    virtual bool ShouldClose() = 0;

    virtual int GetWidth() = 0;
    virtual int GetHeight() = 0;

    virtual bool GetKeyPressed(eKeyValue key) = 0;
    virtual bool GetMouseButtonPressed(eMouseButtonValue button) = 0;
    virtual CursorPos GetCursorPosition() = 0;
};