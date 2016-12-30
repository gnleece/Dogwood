#pragma once

//////////////////////////////////////////////////////////////////////////
// GameObjectBase contains functionality that is used by both runtime and
// toolside implementations of game objects.
//////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>
#include "Math/Transform.h"

using std::string;
using std::vector;

class Collider;
class MeshInstance;
class RigidBody;

class GameObjectBase
{
public:
    GameObjectBase(unsigned int guid, string name, GameObjectBase* parent = NULL);
    ~GameObjectBase();

    unsigned int    GetID();
    string          GetName();
    void            SetName(string name);
    void            SetName(const char* name);

    GameObjectBase* GetParent();
    void            SetParent(GameObjectBase* parent, int index = -1);

    vector<GameObjectBase*>& GetChildren();

    Transform&      GetTransform();
    //void            SetTransform(Transform& t);
    //void            SetTransform(Matrix4x4& m);

    // TODO Rendering/Mesh function should not exist here
    virtual void    Render(bool dirty, bool wireframe = false);
    void            SetMeshInstance(MeshInstance* mesh);
    MeshInstance*   GetMeshInstance();

    vector<Collider*>& GetColliders();
    void            ClearColliders();
    void            AddCollider(Collider* collider);
    void            RemoveCollider(Collider* collider);

    RigidBody*      GetRigidBody();
    void            SetRigidBody(RigidBody* rigidBody);

protected:
    void            AddChild(GameObjectBase* child, int index = -1);
    void            RemoveChild(GameObjectBase* child);

    unsigned int            m_guid;
    string                  m_name;

    Transform               m_transform;

    GameObjectBase*         m_parent;
    vector<GameObjectBase*> m_children;

    MeshInstance*           m_mesh;
    vector<Collider*>       m_colliders;
    RigidBody*              m_rigidBody;
};