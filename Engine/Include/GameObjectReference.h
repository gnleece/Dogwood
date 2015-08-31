#pragma once

#include <unordered_map>

class GameObject;
class GameObjectBase;
class ToolsideGameObject;

using std::unordered_map;

class GameObjectReference
{
public:
    GameObjectReference();
    GameObjectReference(unsigned int guid);

    unsigned int                GetGuid();
    void                        SetGuid(unsigned int guid);

    GameObject*                 GetGameObject();

    static GameObjectBase*      GetGameObjectBase(unsigned int guid);
    static GameObject*          GetGameObject(unsigned int guid);
    static ToolsideGameObject*  GetToolsideGameObject(unsigned int guid);

    static void                 AddToMap(unsigned int guid, GameObjectBase* gameObject);
    static void                 RemoveFromMap(unsigned int guid);

private:
    unsigned int    m_guid;

    static unordered_map<unsigned int, GameObjectBase*> GameObjectMap;
};