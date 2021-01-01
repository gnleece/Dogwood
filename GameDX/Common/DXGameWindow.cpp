#include "pch.h"
#include "DXGameWindow.h"

using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::System;
using namespace Windows::Foundation;

void DXGameWindow::SetCoreWindow(Windows::UI::Core::CoreWindow^ coreWindow)
{
    m_coreWindow = coreWindow;
}

void DXGameWindow::OnPointerPressed(Windows::UI::Core::PointerEventArgs^ args)
{
    
}

void DXGameWindow::PollEvents(float deltaTime)
{
    // TODO implement me
}

void DXGameWindow::SwapBuffers()
{
    // TODO implement me
}

bool DXGameWindow::ShouldClose()
{
    return false; // TODO implement me
}

int DXGameWindow::GetWidth()
{
    return m_width;
}

int DXGameWindow::GetHeight()
{
    return m_height;
}

float DXGameWindow::GetLastFrameTime()
{
    return 0; // TODO implement me
}

bool DXGameWindow::GetKeyPressed(eKeyValue key)
{
    auto keyState = m_coreWindow->GetKeyState(DGWDKeyToWindowsKey(key));
    return (keyState == Windows::UI::Core::CoreVirtualKeyStates::Down);
}

bool DXGameWindow::GetMouseButtonPressed(eMouseButtonValue button)
{
    return false;   // TODO implement me
}

CursorPos DXGameWindow::GetCursorPosition()
{
    auto cursorPoint = m_coreWindow->PointerPosition;
    return CursorPos(cursorPoint.X, cursorPoint.Y);
}

GamePad* DXGameWindow::GetGamePad(unsigned int id)
{
    return NULL; // TODO implement me
}

bool DXGameWindow::EnableGamePad(GamePad* pad, unsigned int id, bool enable)
{
    return false;   // TODO implement me
}

Windows::System::VirtualKey DXGameWindow::DGWDKeyToWindowsKey(eKeyValue dgwdKey)
{
    switch (dgwdKey)
    {
        case DGWD_KEY_SPACE:
            return Windows::System::VirtualKey::Space;

        case DGWD_KEY_0:
            return Windows::System::VirtualKey::Number0;
        case DGWD_KEY_1:
            return Windows::System::VirtualKey::Number1;
        case DGWD_KEY_2:
            return Windows::System::VirtualKey::Number2;
        case DGWD_KEY_3:
            return Windows::System::VirtualKey::Number3;
        case DGWD_KEY_4:
            return Windows::System::VirtualKey::Number4;
        case DGWD_KEY_5:
            return Windows::System::VirtualKey::Number5;
        case DGWD_KEY_6:
            return Windows::System::VirtualKey::Number6;
        case DGWD_KEY_7:
            return Windows::System::VirtualKey::Number7;
        case DGWD_KEY_8:
            return Windows::System::VirtualKey::Number8;
        case DGWD_KEY_9:
            return Windows::System::VirtualKey::Number9;

        case DGWD_KEY_A:
            return Windows::System::VirtualKey::A;
        case DGWD_KEY_B:
            return Windows::System::VirtualKey::B;
        case DGWD_KEY_C:
            return Windows::System::VirtualKey::C;
        case DGWD_KEY_D:
            return Windows::System::VirtualKey::D;
        case DGWD_KEY_E:
            return Windows::System::VirtualKey::E;
        case DGWD_KEY_F:
            return Windows::System::VirtualKey::F;
        case DGWD_KEY_G:
            return Windows::System::VirtualKey::G;
        case DGWD_KEY_H:
            return Windows::System::VirtualKey::H;
        case DGWD_KEY_I:
            return Windows::System::VirtualKey::I;
        case DGWD_KEY_J:
            return Windows::System::VirtualKey::J;
        case DGWD_KEY_K:
            return Windows::System::VirtualKey::K;
        case DGWD_KEY_L:
            return Windows::System::VirtualKey::L;
        case DGWD_KEY_M:
            return Windows::System::VirtualKey::M;
        case DGWD_KEY_N:
            return Windows::System::VirtualKey::N;
        case DGWD_KEY_O:
            return Windows::System::VirtualKey::O;
        case DGWD_KEY_P:
            return Windows::System::VirtualKey::P;
        case DGWD_KEY_Q:
            return Windows::System::VirtualKey::Q;
        case DGWD_KEY_R:
            return Windows::System::VirtualKey::R;
        case DGWD_KEY_S:
            return Windows::System::VirtualKey::S;
        case DGWD_KEY_T:
            return Windows::System::VirtualKey::T;
        case DGWD_KEY_U:
            return Windows::System::VirtualKey::U;
        case DGWD_KEY_V:
            return Windows::System::VirtualKey::V;
        case DGWD_KEY_W:
            return Windows::System::VirtualKey::W;
        case DGWD_KEY_X:
            return Windows::System::VirtualKey::X;
        case DGWD_KEY_Y:
            return Windows::System::VirtualKey::Y;
        case DGWD_KEY_Z:
            return Windows::System::VirtualKey::Z;

        case DGWD_KEY_ESCAPE:
            return Windows::System::VirtualKey::Escape;
        case DGWD_KEY_ENTER:
            return Windows::System::VirtualKey::Enter;
        case DGWD_KEY_TAB:
            return Windows::System::VirtualKey::Tab;
        case DGWD_KEY_BACKSPACE:
            return Windows::System::VirtualKey::Back;
        case DGWD_KEY_INSERT:
            return Windows::System::VirtualKey::Insert;
        case DGWD_KEY_DELETE:
            return Windows::System::VirtualKey::Delete;
        case DGWD_KEY_RIGHT:
            return Windows::System::VirtualKey::Right;
        case DGWD_KEY_LEFT:
            return Windows::System::VirtualKey::Left;
        case DGWD_KEY_DOWN:
            return Windows::System::VirtualKey::Down;
        case DGWD_KEY_UP:
            return Windows::System::VirtualKey::Up;
        case DGWD_KEY_PAGE_UP:
            return Windows::System::VirtualKey::PageUp;
        case DGWD_KEY_PAGE_DOWN:
            return Windows::System::VirtualKey::PageDown;
        case DGWD_KEY_HOME:
            return Windows::System::VirtualKey::Home;
        case DGWD_KEY_END:
            return Windows::System::VirtualKey::End;
        case DGWD_KEY_CAPS_LOCK:
            return Windows::System::VirtualKey::CapitalLock;
        case DGWD_KEY_SCROLL_LOCK:
            return Windows::System::VirtualKey::Scroll;
        case DGWD_KEY_NUM_LOCK:
            return Windows::System::VirtualKey::NumberKeyLock;
        case DGWD_KEY_PRINT_SCREEN:
            return Windows::System::VirtualKey::Print;
        case DGWD_KEY_PAUSE:
            return Windows::System::VirtualKey::Pause;

        case DGWD_KEY_F1:
            return Windows::System::VirtualKey::F1;
        case DGWD_KEY_F2:
            return Windows::System::VirtualKey::F2;
        case DGWD_KEY_F3:
            return Windows::System::VirtualKey::F3;
        case DGWD_KEY_F4:
            return Windows::System::VirtualKey::F4;
        case DGWD_KEY_F5:
            return Windows::System::VirtualKey::F5;
        case DGWD_KEY_F6:
            return Windows::System::VirtualKey::F6;
        case DGWD_KEY_F7:
            return Windows::System::VirtualKey::F7;
        case DGWD_KEY_F8:
            return Windows::System::VirtualKey::F8;
        case DGWD_KEY_F9:
            return Windows::System::VirtualKey::F9;
        case DGWD_KEY_F10:
            return Windows::System::VirtualKey::F10;
        case DGWD_KEY_F11:
            return Windows::System::VirtualKey::F11;
        case DGWD_KEY_F12:
            return Windows::System::VirtualKey::F12;
        case DGWD_KEY_F13:
            return Windows::System::VirtualKey::F13;
        case DGWD_KEY_F14:
            return Windows::System::VirtualKey::F14;
        case DGWD_KEY_F15:
            return Windows::System::VirtualKey::F15;
        case DGWD_KEY_F16:
            return Windows::System::VirtualKey::F16;
        case DGWD_KEY_F17:
            return Windows::System::VirtualKey::F17;
        case DGWD_KEY_F18:
            return Windows::System::VirtualKey::F18;
        case DGWD_KEY_F19:
            return Windows::System::VirtualKey::F19;
        case DGWD_KEY_F20:
            return Windows::System::VirtualKey::F20;
        case DGWD_KEY_F21:
            return Windows::System::VirtualKey::F21;
        case DGWD_KEY_F22:
            return Windows::System::VirtualKey::F22;
        case DGWD_KEY_F23:
            return Windows::System::VirtualKey::F23;
        case DGWD_KEY_F24:
            return Windows::System::VirtualKey::F24;

        case DGWD_KEY_KP_0:
            return Windows::System::VirtualKey::NumberPad0;
        case DGWD_KEY_KP_1:
            return Windows::System::VirtualKey::NumberPad1;
        case DGWD_KEY_KP_2:
            return Windows::System::VirtualKey::NumberPad2;
        case DGWD_KEY_KP_3:
            return Windows::System::VirtualKey::NumberPad3;
        case DGWD_KEY_KP_4:
            return Windows::System::VirtualKey::NumberPad4;
        case DGWD_KEY_KP_5:
            return Windows::System::VirtualKey::NumberPad5;
        case DGWD_KEY_KP_6:
            return Windows::System::VirtualKey::NumberPad6;
        case DGWD_KEY_KP_7:
            return Windows::System::VirtualKey::NumberPad7;
        case DGWD_KEY_KP_8:
            return Windows::System::VirtualKey::NumberPad8;
        case DGWD_KEY_KP_9:
            return Windows::System::VirtualKey::NumberPad9;

        case DGWD_KEY_KP_DECIMAL:
            return Windows::System::VirtualKey::Decimal;
        case DGWD_KEY_KP_DIVIDE:
            return Windows::System::VirtualKey::Divide;
        case DGWD_KEY_KP_MULTIPLY:
            return Windows::System::VirtualKey::Multiply;
        case DGWD_KEY_KP_SUBTRACT:
            return Windows::System::VirtualKey::Subtract;
        case DGWD_KEY_KP_ADD:
            return Windows::System::VirtualKey::Add;

        case DGWD_KEY_LEFT_SHIFT:
            return Windows::System::VirtualKey::LeftShift;
        case DGWD_KEY_LEFT_CONTROL:
            return Windows::System::VirtualKey::LeftControl;
        case DGWD_KEY_LEFT_ALT:
            return Windows::System::VirtualKey::LeftMenu;

        case DGWD_KEY_RIGHT_SHIFT:
            return Windows::System::VirtualKey::RightShift;
        case DGWD_KEY_RIGHT_CONTROL:
            return Windows::System::VirtualKey::RightControl;
        case DGWD_KEY_RIGHT_ALT:
            return Windows::System::VirtualKey::RightMenu;

        case DGWD_KEY_MENU:
            return Windows::System::VirtualKey::Menu;
    }
    return Windows::System::VirtualKey::None;
}