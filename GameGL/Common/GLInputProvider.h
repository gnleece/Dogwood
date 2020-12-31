#pragma once

#include "Input/InputProvider.h"
#include <unordered_map>

using std::unordered_map;

class GLGameWindow;

class GLInputProvider : public InputProvider
{
public:
    GLInputProvider(GLGameWindow* gameWindow);

    void        PollEvents(float deltaTime);

    bool        GetKeyPressed(eKeyValue key);
    bool        GetMouseButtonPressed(eMouseButtonValue button);
    CursorPos   GetCursorPosition();

    GamePad*    GetGamePad(unsigned int id);
    bool        EnableGamePad(GamePad* pad, unsigned int id, bool enable = true);

private:
    GLGameWindow* m_gameWindow;
    unordered_map<unsigned int, GamePad*> m_gamePads;
};