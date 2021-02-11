#pragma once
//#include "pch.h"
#include "Window/GameWindow.h"

#include <unordered_map>

using std::unordered_map;

class GamePad;

class DXGameWindow : public GameWindow
{
public:
    DXGameWindow();

    void SetCoreWindow(Windows::UI::Core::CoreWindow^ coreWindow);
    void OnPointerPressed(Windows::UI::Core::PointerEventArgs^ args);
    void OnPointerReleased(Windows::UI::Core::PointerEventArgs^ args);

    void PollEvents(float deltaTime);

    void SwapBuffers();
    bool ShouldClose();

    int GetWidth();
    int GetHeight();

    float GetLastFrameTime();

    bool        GetKeyPressed(eKeyValue key);
    bool        GetMouseButtonPressed(eMouseButtonValue button);
    CursorPos   GetCursorPosition();

    GamePad*    GetGamePad(unsigned int id);
    bool        EnableGamePad(GamePad* pad, unsigned int id, bool enable = true);

private:
    static Windows::System::VirtualKey DGWDKeyToWindowsKey(eKeyValue dgwdKey);

    void ClearPointerPressedStates();
    void UpdatePointerPressedStates(Windows::UI::Core::PointerEventArgs^ args);

    Platform::Agile<Windows::UI::Core::CoreWindow> m_coreWindow;

    int m_width;
    int m_height;

    unordered_map<int, bool> m_pointerPressedStates;
};