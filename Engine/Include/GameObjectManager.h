#pragma once

#include <queue>
#include <vector>

using std::queue;
using std::vector;

class GameObject;

class GameObjectManager
{
public:
    static GameObjectManager& Singleton()
    {
        static GameObjectManager singleton;
        return singleton;
    }
    GameObjectManager() {}

    void    Startup();
    void    Shutdown();
    void    Update(float deltaTime);

    void    Register(GameObject* gameObject);
    void    SetActive(GameObject* gameObject, bool active);
    void    Destroy(GameObject* gameObject);

private:
    void    DestroyInternal(GameObject* gameObject);

    queue<GameObject*>      m_createQueue;
    queue<GameObject*>      m_startQueue;
    queue<GameObject*>      m_activateQueue;
    queue<GameObject*>      m_deactivateQueue;
    queue<GameObject*>      m_destroyQueue;

    vector<GameObject*>     m_activeList;

};