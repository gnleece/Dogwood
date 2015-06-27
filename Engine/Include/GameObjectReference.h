#pragma once

#include <unordered_map>

class GameObject;

using std::unordered_map;

class GameObjectReference
{
public:
    GameObjectReference();
    GameObjectReference(unsigned int guid);

    unsigned int        GetGuid();
    void                SetGuid(unsigned int guid);
    
    GameObject*         GetGameObject();

    static GameObject*  GetGameObject(unsigned int guid);
    static void         AddToMap(unsigned int guid, GameObject* gameObject);
    static void         RemoveFromMap(unsigned int guid);

private:
    unsigned int        m_guid;

    static unordered_map<unsigned int, GameObject*> GameObjectMap;
};