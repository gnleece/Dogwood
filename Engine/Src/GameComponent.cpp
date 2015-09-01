#include "GameComponent.h"

#include <stdio.h>

GameComponent::GameComponent()
    : m_gameObject(NULL)
{}

bool GameComponent::IsEnabled()
{
    return m_enabled;
}

GameObject* GameComponent::GetGameObject()
{
    return m_gameObject;
}

void GameComponent::SetGameObject(GameObject* gameObject)
{
    m_gameObject = gameObject;
}

void GameComponent::SetEnabled(bool enabled)
{
    m_enabled = false;
}