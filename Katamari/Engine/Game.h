#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include <list>
#include <string>

#include "Window\GameWindow.h"

using std::list;
using std::string;

class GameObject;

class Game
{
public:
    static Game& Singleton()
    {
        static Game singleton;
        return singleton;
    }
    Game() {}

    void Init(string name, int windowWidth, int windowHeight);
    void Run(GameObject* sceneRoot);
    
private:
    void Shutdown();

    GameWindow        m_gameWindow;

    list<GameObject*> m_gameObjects;
    GameObject*       m_rootObject;
};