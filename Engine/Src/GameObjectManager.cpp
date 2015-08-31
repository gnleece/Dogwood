#include "GameObjectManager.h"
#include "GameObject.h"

void GameObjectManager::Startup()
{

}

void GameObjectManager::Shutdown()
{

}

void GameObjectManager::RegisterActiveGameObject(GameObject* gameObject)
{
    m_activeGameObjects.push_back(gameObject);
}

void GameObjectManager::UpdateActiveGameObjects(float deltaTime)
{
    // Call Update() on all active game objects
    std::vector<GameObject*>::iterator goIter;
    for (goIter = m_activeGameObjects.begin(); goIter != m_activeGameObjects.end(); goIter++)
    {
        GameObject* gameObject = *goIter;
        gameObject->Update((float)deltaTime);
    }
}