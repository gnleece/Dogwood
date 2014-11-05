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
class GameProject;

class Game
{
public:
    const static int MAX_FPS = 60;

    static Game& Singleton()
    {
        static Game singleton;
        return singleton;
    }
    Game() {}

    void Init(string projectPath);
    void Run(GameObject* sceneRoot);
    
private:
    void Shutdown();
    void UpdateTime();

    GameProject*      m_gameProject;
    GameWindow        m_gameWindow;
    GameObject*       m_rootObject;

    double            m_prevFrameEndTime;
    double            m_deltaTime;
    double            m_minFrameTime;
};