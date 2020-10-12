#ifdef GRAPHICS_DX

#include "Window/GameWindow.h"
#include "Window/DX/DXGameWindow.h"

#include "Math/Algebra.h"

GameWindow* GameWindow::Create()
{
    return DXGameWindow::Create();
}

void GameWindow::Destroy(GameWindow* gameWindow)
{
    delete gameWindow;
}

DXGameWindow* DXGameWindow::Create()
{
    return new DXGameWindow();
}

void DXGameWindow::Setup(string name, int width, int height)
{
    
}

void DXGameWindow::Destroy()
{

}

void DXGameWindow::SwapBuffers()
{

}

bool DXGameWindow::ShouldClose()
{
    return false;
}

int DXGameWindow::GetWidth()
{
    return 0;
}

int DXGameWindow::GetHeight()
{
    return 0;
}

bool DXGameWindow::GetKeyPressed(eKeyValue key)
{
    return false;
}

bool DXGameWindow::GetMouseButtonPressed(eMouseButtonValue button)
{
    return false;
}

CursorPos DXGameWindow::GetCursorPosition()
{
    return CursorPos(0, 0);
}

float DXGameWindow::GetLastFrameTime()
{
    return 0;
}

#endif