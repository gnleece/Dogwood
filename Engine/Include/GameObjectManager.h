#pragma once

#include <vector>

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

    void    RegisterActiveGameObject(GameObject* gameObject);
    void    UpdateActiveGameObjects(float deltaTime);

private:
    vector<GameObject*>     m_activeGameObjects;
};