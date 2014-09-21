#include "GameObject.h"

#include "Rendering\MeshInstance.h"
#include "GameComponent.h"

vector<GameObject*> GameObject::ActiveGameObjects = vector<GameObject*>();

GameObject::GameObject(string name, GameObject* parent)
 : m_dirty(true)
{
    m_name = name;
    SetParent(parent);

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

void GameObject::SetParent(GameObject* parent, int index)
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
}

void GameObject::AddComponent(GameComponent* component)
{
    if (component)
    {
        component->SetGameObject(this);
        m_components.push_back(component);
    }
}

GameObject* GameObject::GetChild(int index)
{
    if (index >= m_children.size())
        return NULL;

    return m_children[index];
}

int GameObject::GetChildNumber()
{
    // determine the index of this game object in the parent's list of children
    if (m_parent)
    {
        std::vector<GameObject*>::iterator iter = std::find(m_parent->m_children.begin(), m_parent->m_children.end(), const_cast<GameObject*>(this));
        return std::distance(m_parent->m_children.begin(), iter);
    }

    return 0;
}

int GameObject::GetChildCount()
{
    return m_children.size();
}

bool GameObject::InsertChildren(int position, int count)
{
    if (position < 0)
        return false;

    for (int i = 0; i < count; i++)
    {
        GameObject* child = new GameObject("", NULL);
        child->SetParent(this, position);
        child->SetName("GameObject");
    }

    return true;
}

bool GameObject::InsertChild(int position, GameObject* child)
{
    if (child == NULL || position < 0)
        return false;

    child->SetParent(this, position);
}

bool GameObject::RemoveChildren(int position, int count)
{
    if (position < 0 || position + count > m_children.size())
        return false;

    m_children.erase(m_children.begin() + position, m_children.begin() + position + count);

    return true;
}

GameObject* GameObject::DeepCopy(GameObject* parent)
{
    GameObject* newGO = new GameObject(m_name, parent);
    newGO->SetLocalTransform(m_localTransform);
    // TODO set id

    if (m_mesh != NULL)
    {
        newGO->SetMesh(m_mesh->DeepCopy());
    }

    // TODO copy other components

    std::vector<GameObject*>::iterator childIter;
    for (childIter = m_children.begin(); childIter != m_children.end(); childIter++)
    {
        GameObject* child = *childIter;
        child->DeepCopy(newGO);
    }

    return newGO;
}

void GameObject::Start()
{
    // start all components
    std::vector<GameComponent*>::iterator compIter;
    for (compIter = m_components.begin(); compIter != m_components.end(); compIter++)
    {
        GameComponent* component = *compIter;
        component->Start();
    }
}

void GameObject::Update(float deltaTime)
{
    // update all components
    std::vector<GameComponent*>::iterator compIter;
    for (compIter = m_components.begin(); compIter != m_components.end(); compIter++)
    {
        GameComponent* component = *compIter;
        component->Update(deltaTime);
    }
}

void GameObject::Render(Transform& parentWorldTransform, bool dirty)
{
    dirty |= m_dirty | m_localTransform.HasChanged();
    if (dirty)
    {
        // hierarchy has changed so recompute world transform (and cache it)
        m_worldTransform = parentWorldTransform*m_localTransform;
        m_dirty = false;
        m_localTransform.ClearChangedFlag();
    }

    // render the mesh, if there is one
    if (m_mesh)
    {
        m_mesh->Render(m_worldTransform);
    }

    // render children
    std::vector<GameObject*>::iterator childIter;
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

void GameObject::AddChild(GameObject* child, int index)
{
    if (index >= 0 && index < m_children.size())
    {
        m_children.insert(m_children.begin() + index, child);
    }
    else
    {
        m_children.push_back(child);
    }
}

void GameObject::RemoveChild(GameObject* child)
{
    // TODO fixme
    //m_children.remove(child);
}