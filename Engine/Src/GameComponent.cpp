#include "GameComponent.h"

GameObject* GameComponent::GetGameObject()
{
    return m_gameObject;
}

void GameComponent::SetGameObject(GameObject* gameObject)
{
    m_gameObject = gameObject;
}