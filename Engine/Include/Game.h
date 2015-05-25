#pragma once


#include <list>
#include <string>

#include "Window\GameWindow.h"

using std::list;
using std::string;

class GameComponentFactory;
class GameObject;

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

    void Init(string projectPath, GameComponentFactory* componentFactory);
    void Run(GameObject* sceneRoot);
    
private:
    void Shutdown();
    void UpdateTime();

    GameWindow        m_gameWindow;
    GameObject*       m_rootObject;

    double            m_prevFrameEndTime;
    double            m_deltaTime;
    double            m_minFrameTime;
};