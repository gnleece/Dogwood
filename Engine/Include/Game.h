#pragma once


#include <list>
#include <string>

#include "Scene\Scene.h"
#include "Window\GameWindow.h"

using std::list;
using std::string;

class GameComponentFactory;
class GameObject;

class Game
{
public:
    const static int MAX_FPS = 60;
    const static int FPS_SNAPSHOT_SIZE = 30;

    static Game& Singleton()
    {
        static Game singleton;
        return singleton;
    }
    Game() {}

    void Init(string projectPath, GameComponentFactory* componentFactory);
    void Run(Scene* scene);
    
private:
    void Shutdown();
    void UpdateTime();

    GameWindow  m_gameWindow;
    GameObject* m_rootObject;

    float       m_prevFrameEndTime;
    float       m_deltaTime;
    float       m_minFrameTime;
    float       m_timeSinceFPSSnapshot;
    int         m_framesSinceFPSSnapshot;
};