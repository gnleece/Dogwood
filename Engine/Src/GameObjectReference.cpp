#include "GameObjectReference.h"

unordered_map<unsigned int, GameObject*> GameObjectReference::GameObjectMap = unordered_map<unsigned int, GameObject*>();

GameObjectReference::GameObjectReference()
{
}

GameObjectReference::GameObjectReference(unsigned int guid)
{
    SetGuid(guid);
}

unsigned int GameObjectReference::GetGuid()
{
    return m_guid;
}

void GameObjectReference::SetGuid(unsigned int guid)
{
    m_guid = guid;
}

GameObject* GameObjectReference::GetGameObject()
{
    return GetGameObject(m_guid);
}

GameObject* GameObjectReference::GetGameObject(unsigned int guid)
{
    return GameObjectMap[guid];
}

void GameObjectReference::AddToMap(unsigned int guid, GameObject* gameObject)
{
    GameObjectMap[guid] = gameObject;
}

void GameObjectReference::RemoveFromMap(unsigned int guid)
{
    GameObjectMap.erase(guid);
}