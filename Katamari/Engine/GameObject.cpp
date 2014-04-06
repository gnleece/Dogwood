#include "GameObject.h"

#include "GameComponent.h"

void GameObject::Start()
{
    // start all components
    std::list<GameComponent*>::iterator compIter;
    for (compIter = m_components.begin(); compIter != m_components.end(); compIter++)
    {
        GameComponent* component = *compIter;
        component->Start();
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
}