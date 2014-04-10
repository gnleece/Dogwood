#pragma once

#include <list>
#include <string>

#include "Math\Algebra.h"

using std::string;
using std::list;

class GameComponent;
class MeshInstance;

class GameObject
{
public:
    GameObject();

    const Matrix4x4& GetLocalTransform()        { return m_localTransform; }
    void SetLocalTransform(const Matrix4x4& m);

    int GetID()                                 { return m_id; }
    string GetName()                            { return m_name; }
    void SetName(string name)                   { m_name = name; }

    void SetParent(GameObject* parent);

    void Start();
    void Update(float deltaTime);
    
    void OnEnable();
    void OnDisable();

    void Render(Matrix4x4 parentWorldTransform, bool dirty);

    void SetMesh(MeshInstance* mesh); 
    MeshInstance* GetMesh();

private:
    void AddChild(GameObject* child);
    void RemoveChild(GameObject* child);

    int     m_id;
    string  m_name;

    Matrix4x4               m_localTransform;
    Matrix4x4               m_worldTransform;
    bool                    m_dirty;

    MeshInstance*           m_mesh;         // TODO this should be part of regular component list

    GameObject*             m_parent;
    list<GameObject*>       m_children;

    list<GameComponent*>    m_components;
};