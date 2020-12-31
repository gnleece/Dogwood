#pragma once

#include <string>
#include "Input/Input.h"

using std::string;

class GameWindow
{
public:
    virtual void SwapBuffers() = 0;
    virtual bool ShouldClose() = 0;

    virtual int GetWidth() = 0;
    virtual int GetHeight() = 0;

    virtual float GetLastFrameTime() = 0;
};