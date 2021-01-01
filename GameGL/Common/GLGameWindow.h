#pragma once

#include "Window/GameWindow.h"

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

class GamePad;

class GLGameWindow : public GameWindow
{
public:
    GLGameWindow(string name, int width, int height);
    void Destroy();

    void PollEvents(float deltaTime);

    void SwapBuffers();
    bool ShouldClose();

    int GetWidth();
    int GetHeight();

    float GetLastFrameTime();

    bool GetKeyPressed(eKeyValue key);
    bool GetMouseButtonPressed(eMouseButtonValue button);
    CursorPos GetCursorPosition();

    GamePad* GetGamePad(unsigned int id);
    bool     EnableGamePad(GamePad* pad, unsigned int id, bool enable = true);

private:
    static void ErrorCallback(int error, const char* description);

    static int DGWDKeyToGLFWKey(eKeyValue dgwdKey);
    static int DGWDMouseButtonToGLFWMouseButton(eMouseButtonValue dgwdButton);

    GLFWwindow* m_window;

    string      m_name;
    int         m_width;
    int         m_height;

    unordered_map<unsigned int, GamePad*> m_gamePads;
};