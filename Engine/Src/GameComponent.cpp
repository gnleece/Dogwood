#include "GameComponent.h"

#include <stdio.h>

GameComponent::GameComponent()
    : m_gameObject(NULL)
{}

GameObject* GameComponent::GetGameObject()
{
    return m_gameObject;
}

void GameComponent::SetGameObject(GameObject* gameObject)
{
    m_gameObject = gameObject;
}