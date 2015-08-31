#include "GameObjectBase.h"
#include "GameObjectReference.h"

#include <algorithm>

GameObjectBase::GameObjectBase(unsigned int guid, string name, GameObjectBase* parent)
 : m_dirty(true)
{
    m_guid = guid;
    m_name = name;
    m_parent = parent;

    GameObjectReference::AddToMap(guid, this);
}

GameObjectBase::~GameObjectBase()
{

}

unsigned int GameObjectBase::GetID()
{
    return m_guid;
}

string GameObjectBase::GetName()
{
    return m_name;
}

void GameObjectBase::SetName(string name)
{
    m_name = name;
}

void GameObjectBase::SetName(const char* name)
{
    if (name)
    {
        m_name = name;
    }
}

GameObjectBase* GameObjectBase::GetParent()
{
    return m_parent;
}

void GameObjectBase::SetParent(GameObjectBase* parent, int index)
{
    // if this object had a parent already, remove it from that parent's list of children
    if (m_parent)
    {
        m_parent->RemoveChild(this);
    }

    m_parent = parent;

    // add this object to the new parent's list of children
    if (m_parent)
    {
        m_parent->AddChild(this, index);
    }

    m_dirty = true;
}

vector<GameObjectBase*>& GameObjectBase::GetChildren()
{
    return m_children;
}

Transform& GameObjectBase::GetLocalTransform()
{
    return m_localTransform;
}

Transform GameObjectBase::GetWorldTransform()
{
    // TODO optimize this
    if (m_parent)
    {
        return m_parent->GetWorldTransform()*m_localTransform;
    }

    return m_localTransform;
}

void GameObjectBase::SetLocalTransform(Transform& t)
{
    m_localTransform = t;
    m_dirty = true;
}

void GameObjectBase::SetLocalTransform(Matrix4x4& m)
{
    m_localTransform.SetMatrix(m);
    m_dirty = true;
}

void GameObjectBase::Render(Transform& parentWorldTransform, bool dirty, bool wireframe)
{
}

void GameObjectBase::SetMesh(MeshInstance* mesh)
{
    // clear previous mesh
    if (m_mesh != NULL)
    {
        delete m_mesh;
    }

    // set new mesh
    m_mesh = mesh;
    if (m_mesh != NULL)
    {
        m_mesh->SetGameObject(this);
    }
}

MeshInstance* GameObjectBase::GetMesh()
{
    return m_mesh;
}

void GameObjectBase::AddChild(GameObjectBase* child, int index)
{
    if (index >= 0 && index < (int)m_children.size())
    {
        m_children.insert(m_children.begin() + index, child);
    }
    else
    {
        m_children.push_back(child);
    }
}

void GameObjectBase::RemoveChild(GameObjectBase* child)
{
    m_children.erase(
        std::remove(m_children.begin(), m_children.end(), child),
        m_children.end());
}