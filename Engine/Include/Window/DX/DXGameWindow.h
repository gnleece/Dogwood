#pragma once

#ifdef GRAPHICS_DX

#include "Window/GameWindow.h"

#include <string>

using std::string;

class DXGameWindow : public GameWindow
{
public:
    static DXGameWindow* Create();

    virtual void Setup(string name, int width, int height);
    virtual void Destroy();

    virtual void SwapBuffers();
    virtual bool ShouldClose();

    virtual int GetWidth();
    virtual int GetHeight();

    virtual bool GetKeyPressed(eKeyValue key);
    virtual bool GetMouseButtonPressed(eMouseButtonValue button);
    virtual CursorPos GetCursorPosition();

    virtual float GetLastFrameTime();
};

#endif