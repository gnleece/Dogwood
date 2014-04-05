#include "GameObject.h"

void GameObject::Start()
{
    // start all components
    std::list<GameComponent*>::iterator compIter;
    for (compIter = m_components.begin(); compIter != m_components.end(); compIter++)
    {
        GameComponent* component = *compIter;
        component->Start();
    }

    // start children
    std::list<GameObject*>::iterator childIter;
    for (childIter = m_children.begin(); childIter != m_children.end(); childIter++)
    {
        GameObject* child = *childIter;
        child->Start();
    }
}

void GameObject::Update(float deltaTime)
{
    // update all components
    std::list<GameComponent*>::iterator compIter;
    for (compIter = m_components.begin(); compIter != m_components.end(); compIter++)
    {
        GameComponent* component = *compIter;
        component->Update(deltaTime);
    }
    
    // update children
    std::list<GameObject*>::iterator childIter;
    for (childIter = m_children.begin(); childIter != m_children.end(); childIter++)
    {
        GameObject* child = *childIter;
        child->Update(deltaTime);
    }
}

void GameObject::Render()
{
    // TODO render self

    // render children
    std::list<GameObject*>::iterator iter;
    for (iter = m_children.begin(); iter != m_children.end(); iter++)
    {
        GameObject* child = *iter;
        child->Render();
    }
}