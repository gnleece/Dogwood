#pragma once

//////////////////////////////////////////////////////////////////////////
// GameObjectBase contains functionality that is used by both runtime and
// toolside implementations of game objects.
//////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>
#include "Math/Transform.h"
#include "Rendering/MeshInstance.h"

using std::string;
using std::vector;

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

    Transform&      GetLocalTransform();
    Transform       GetWorldTransform();
    void            SetLocalTransform(Transform& t);
    void            SetLocalTransform(Matrix4x4& m);

    // TODO Rendering/Mesh function should not exist here
    virtual void    Render(Transform& parentWorldTransform, bool dirty, bool wireframe = false);
    void            SetMesh(MeshInstance* mesh);
    MeshInstance*   GetMesh();

protected:
    void            AddChild(GameObjectBase* child, int index = -1);
    void            RemoveChild(GameObjectBase* child);

    unsigned int            m_guid;
    string                  m_name;

    GameObjectBase*         m_parent;
    vector<GameObjectBase*> m_children;

    Transform               m_localTransform;
    Transform               m_worldTransform;
    bool                    m_dirty;

    MeshInstance*           m_mesh;         // TODO this shouldn't be here
};