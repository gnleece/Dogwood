#include "GameObjectReference.h"

unordered_map<unsigned int, GameObjectBase*> GameObjectReference::GameObjectMap = unordered_map<unsigned int, GameObjectBase*>();

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

GameObjectBase* GameObjectReference::GetGameObjectBase(unsigned int guid)
{
    return GameObjectMap[guid];
}

GameObject* GameObjectReference::GetGameObject(unsigned int guid)
{
    return (GameObject*)GetGameObjectBase(guid);
}

ToolsideGameObject* GameObjectReference::GetToolsideGameObject(unsigned int guid)
{
    return (ToolsideGameObject*)GetGameObjectBase(guid);
}

void GameObjectReference::AddToMap(unsigned int guid, GameObjectBase* gameObject)
{
    GameObjectMap[guid] = gameObject;
}

void GameObjectReference::RemoveFromMap(unsigned int guid)
{
    GameObjectMap.erase(guid);
}