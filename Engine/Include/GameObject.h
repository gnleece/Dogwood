#pragma once

#include <string>
#include <vector>

#include "Math\Algebra.h"
#include "Math\Transform.h"

using std::string;
using std::vector;

class GameComponent;
class MeshInstance;

class GameObject
{
public:
    GameObject(string name = "", GameObject* parent = NULL);

    Transform& GetLocalTransform()        { return m_localTransform; }
    void SetLocalTransform(Transform& t);
    void SetLocalTransform(Matrix4x4& m);

    int GetID()                                 { return m_id; }
    string GetName()                            { return m_name; }
    void SetName(string name)                   { m_name = name; }
    void SetName(const char* name)              { if (name) m_name = name; }

    GameObject* GetParent()                     { return m_parent; }
    void SetParent(GameObject* parent, int index = -1);
    void AddComponent(GameComponent* component);

    GameObject* GetChild(int index);
    vector<GameObject*>& GetChildren();
    int GetChildNumber();
    int GetChildCount();

    bool InsertChildren(int position, int count);
    bool InsertChild(int position, GameObject* child);  // TODO this needs some cleanup
    bool RemoveChildren(int position, int count);

    GameObject* DeepCopy(GameObject* parent = NULL);

    void Start();
    void Update(float deltaTime);
    void OnEnable();
    void OnDisable();

    void Render(Transform& parentWorldTransform, bool dirty, bool wireframe = false);

    void SetMesh(MeshInstance* mesh); 
    MeshInstance* GetMesh();

    void SetSelected(bool selected);

    static vector<GameObject*> ActiveGameObjects;

private:
    void AddChild(GameObject* child, int index = -1);
    void RemoveChild(GameObject* child);

    int                     m_id;
    string                  m_name;

    Transform               m_localTransform;
    Transform               m_worldTransform;
    bool                    m_dirty;
    bool                    m_selected;

    MeshInstance*           m_mesh;         // TODO this should be part of regular component list

    GameObject*             m_parent;
    vector<GameObject*>     m_children;

    vector<GameComponent*>  m_components;
};