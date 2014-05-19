#include "GameObject.h"

#include "Rendering\MeshInstance.h"
#include "GameComponent.h"

list<GameObject*> GameObject::ActiveGameObjects = list<GameObject*>();

GameObject::GameObject() : m_dirty(true)
{
    // TODO unregister on destroy
    ActiveGameObjects.push_back(this);
}

void GameObject::SetLocalTransform(Transform& t)
{
    m_localTransform = t;
    m_dirty = true;
}

void GameObject::SetLocalTransform(Matrix4x4& m)
{
    m_localTransform.SetMatrix(m);
    m_dirty = true;
}

void GameObject::SetParent(GameObject* parent)
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
        m_parent->AddChild(this);
    }
}

void GameObject::AddComponent(GameComponent* component)
{
    if (component)
    {
        component->SetGameObject(this);
        m_components.push_back(component);
    }
}

void GameObject::Start()
{
    // start all components
    std::list<GameComponent*>::iterator compIter;
    for (compIter = m_components.begin(); compIter != m_components.end(); compIter++)
    {
        GameComponent* component = *compIter;
        component->Start();
    }
}

void GameObject::Update(float deltaTime)
{
    // update all components
    std::list<GameComponent*>::iterator compIter;
    for (compIter = m_components.begin(); compIter != m_components.end(); compIter++)
    {
        GameComponent* component = *compIter;
        component->Update(deltaTime);
    }
}

void GameObject::Render(Transform& parentWorldTransform, bool dirty)
{
    dirty |= m_dirty;
    if (dirty)
    {
        // hierarchy has changed so recompute world transform (and cache it)
        m_worldTransform = parentWorldTransform*m_localTransform;
        m_dirty = false;
    }

    // render the mesh, if there is one
    if (m_mesh)
    {
        m_mesh->Render(m_worldTransform);
    }

    // render children
    std::list<GameObject*>::iterator childIter;
    for (childIter = m_children.begin(); childIter != m_children.end(); childIter++)
    {
        GameObject* child = *childIter;
        child->Render(m_worldTransform, dirty);
    }
}

void GameObject::SetMesh(MeshInstance* mesh)
{
    m_mesh = mesh;
}

MeshInstance* GameObject::GetMesh()
{
    return m_mesh;
}

void GameObject::AddChild(GameObject* child)
{
    m_children.push_back(child);
}

void GameObject::RemoveChild(GameObject* child)
{
    m_children.remove(child);
}