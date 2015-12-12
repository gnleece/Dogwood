#include "GameObject.h"

#include "GameComponent.h"
#include "GameObjectManager.h"
#include "GameObjectReference.h"

#include "Rendering/MeshInstance.h"

GameObject::GameObject(unsigned int guid, string name, GameObjectBase* parent)
 : GameObjectBase(guid, name, parent), m_active(true)
{
    SetParent(parent);

    // TODO unregister on destroy
    GameObjectManager::Singleton().Register(this);
}

void GameObject::AddComponent(GameComponent* component)
{
    if (component)
    {
        component->SetGameObject(this);
        m_components.push_back(component);
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

void GameObject::OnCreate()
{
    printf("\t%s OnCreate\n", m_name.c_str());

    // Notify all components of Create
    std::vector<GameComponent*>::iterator compIter;
    for (compIter = m_components.begin(); compIter != m_components.end(); compIter++)
    {
        GameComponent* component = *compIter;
        component->OnCreate();
    }

    printf("\n");
}

void GameObject::OnDestroy()
{
    // Notify all components of Destroy
    std::vector<GameComponent*>::iterator compIter;
    for (compIter = m_components.begin(); compIter != m_components.end(); compIter++)
    {
        GameComponent* component = *compIter;
        component->OnDestroy();
    }
}

void GameObject::OnStart()
{
    printf("\t%s OnStart\n", m_name.c_str());

    // Notify all components of Start
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

void GameObject::OnActivate()
{
    // Notify all components of Activate (Enable)
    std::vector<GameComponent*>::iterator compIter;
    for (compIter = m_components.begin(); compIter != m_components.end(); compIter++)
    {
        GameComponent* component = *compIter;
        component->OnEnable();
    }
}

void GameObject::OnDeactivate()
{
    // Notify all components of Deactivate (disable)
    std::vector<GameComponent*>::iterator compIter;
    for (compIter = m_components.begin(); compIter != m_components.end(); compIter++)
    {
        GameComponent* component = *compIter;
        component->OnDisable();
    }
}

void GameObject::Render(bool dirty, bool wireframe)
{
    // Render the mesh
    if (m_mesh)
    {
        m_mesh->Render(m_transform, wireframe);
    }

    // Render children
    std::vector<GameObjectBase*>::iterator childIter;
    for (childIter = m_children.begin(); childIter != m_children.end(); childIter++)
    {
        GameObjectBase* child = *childIter;
        child->Render(dirty, wireframe);
    }
}