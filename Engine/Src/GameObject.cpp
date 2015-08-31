#include "GameObject.h"

#include "GameComponent.h"
#include "GameObjectManager.h"
#include "GameObjectReference.h"

GameObject::GameObject(unsigned int guid, string name, GameObjectBase* parent)
 : GameObjectBase(guid, name, parent)
{
    SetParent(parent);

    // TODO unregister on destroy
    GameObjectManager::Singleton().RegisterActiveGameObject(this);
}

void GameObject::AddComponent(GameComponent* component)
{
    if (component)
    {
        component->SetGameObject(this);
        m_components.push_back(component);
    }
}

void GameObject::OnStart()
{
    // Start all components
    std::vector<GameComponent*>::iterator compIter;
    for (compIter = m_components.begin(); compIter != m_components.end(); compIter++)
    {
        GameComponent* component = *compIter;
        component->OnStart();
    }
}

void GameObject::Update(float deltaTime)
{
    // Update all components
    std::vector<GameComponent*>::iterator compIter;
    for (compIter = m_components.begin(); compIter != m_components.end(); compIter++)
    {
        GameComponent* component = *compIter;
        component->Update(deltaTime);
    }
}

void GameObject::SetActive(bool active)
{
    // TODO implement me
}

bool GameObject::IsActiveSelf()
{
    return m_active;
}

bool GameObject::IsActiveInHierarchy()
{
    // TODO implement properly
    return m_active;
}

void GameObject::Render(Transform& parentWorldTransform, bool dirty, bool wireframe)
{
    // Determine worldspace transform
    dirty |= m_dirty | m_localTransform.HasChanged();
    if (dirty)
    {
        // Hierarchy has changed so recompute world transform (and cache it)
        m_worldTransform = parentWorldTransform*m_localTransform;
        m_dirty = false;
        m_localTransform.ClearChangedFlag();
    }

    // Render the mesh
    if (m_mesh)
    {
        m_mesh->Render(m_worldTransform, wireframe);
    }

    // Render children
    std::vector<GameObjectBase*>::iterator childIter;
    for (childIter = m_children.begin(); childIter != m_children.end(); childIter++)
    {
        GameObjectBase* child = *childIter;
        child->Render(m_worldTransform, dirty, wireframe);
    }
}