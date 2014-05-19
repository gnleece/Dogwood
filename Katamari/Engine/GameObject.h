#pragma once

#include <list>
#include <string>

#include "Math\Algebra.h"
#include "Math\Transform.h"

using std::string;
using std::list;

class GameComponent;
class MeshInstance;

class GameObject
{
public:
    GameObject();

    Transform& GetLocalTransform()        { return m_localTransform; }
    void SetLocalTransform(Transform& t);
    void SetLocalTransform(Matrix4x4& m);

    int GetID()                                 { return m_id; }
    string GetName()                            { return m_name; }
    void SetName(string name)                   { m_name = name; }

    void SetParent(GameObject* parent);
    void AddComponent(GameComponent* component);

    void Start();
    void Update(float deltaTime);
    
    void OnEnable();
    void OnDisable();

    void Render(Transform& parentWorldTransform, bool dirty);

    void SetMesh(MeshInstance* mesh); 
    MeshInstance* GetMesh();

    static list<GameObject*> ActiveGameObjects;

private:
    void AddChild(GameObject* child);
    void RemoveChild(GameObject* child);

    int     m_id;
    string  m_name;

    Transform               m_localTransform;
    Transform               m_worldTransform;
    bool                    m_dirty;

    MeshInstance*           m_mesh;         // TODO this should be part of regular component list

    GameObject*             m_parent;
    list<GameObject*>       m_children;

    list<GameComponent*>    m_components;
};