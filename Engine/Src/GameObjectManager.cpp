#include "GameObjectManager.h"
#include "GameObject.h"

void GameObjectManager::Startup()
{}

void GameObjectManager::Shutdown()
{}

void GameObjectManager::Update(float deltaTime)
{
    // Process Destroy queue --
    // This must be done first so that these objects don't get Updated this frame
    while(m_destroyQueue.size() > 0)
    {
        GameObject* gameObject = m_destroyQueue.front();
        m_destroyQueue.pop();
        DestroyInternal(gameObject);
    }

    // Process Deactivate queue --
    // This must be done before Update() so that objects that are newly deactivated aren't Updated this frame
    while (m_deactivateQueue.size() > 0)
    {
        GameObject* gameObject = m_deactivateQueue.front();
        m_deactivateQueue.pop();
        gameObject->OnDeactivate();
    }

    // Update all objects in the Active list --
    // This must be done before processing Start queue, so that objects that were just started aren't Updated this frame
    std::vector<GameObject*>::iterator goIter;
    for (goIter = m_activeList.begin(); goIter != m_activeList.end(); goIter++)
    {
        GameObject* gameObject = *goIter;
        gameObject->Update((float)deltaTime);
    }

    // Process Start queue --
    // This must be done before processing Create queue, so that objects that were just created aren't Started this
    while (m_startQueue.size() > 0)
    {
        GameObject* gameObject = m_startQueue.front();
        m_startQueue.pop();
        gameObject->OnStart();

        // If the object is active, add it to the list of active objects
        if (gameObject->IsActiveInHierarchy())
        {
            m_activeList.push_back(gameObject);
        }
    }

    // Process Create queue
    while (m_createQueue.size() > 0)
    {
        GameObject* gameObject = m_createQueue.front();
        m_createQueue.pop();
        gameObject->OnCreate();

        // Move object to the Start queue
        m_startQueue.push(gameObject);
    }

    // Process Activate queue --
    // This must be done after Update() so that objects that are newly Activated aren't updated until next frame
    while (m_activateQueue.size() > 0)
    {
        GameObject* gameObject = m_activateQueue.front();
        m_activateQueue.pop();
        gameObject->OnActivate();

        // Move object to the Active list
        m_activeList.push_back(gameObject);
    }
}

void GameObjectManager::Register(GameObject* gameObject)
{
    m_createQueue.push(gameObject);
}

void GameObjectManager::DestroyInternal(GameObject* gameObject)
{
    // TODO implement me
}