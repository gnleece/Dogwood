#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include <list>
#include <string>

#include "Rendering\RenderManager.h"

using std::list;

class GameObject;
class Texture;

class Game
{
public:
    Game(std::string name, int windowWidth, int windowHeight);
    void Run();
    void Shutdown();

private:
    void WindowSetup();
    void WindowCleanup();

    void BuildTestScene();      // TODO just for testing, remove me
    void DeleteTestScene();     // TODO just for testing, remove me

    std::string     m_name;

    GLFWwindow*     m_window;
    int             m_windowWidth;
    int             m_windowHeight;

    list<GameObject*> m_gameObjects;
    GameObject*       m_rootObject;
};