#include "ToolsideGameObject.h"

#include <algorithm>

#include "ToolsideGameComponent.h"
#include "Util.h"
#include "Math\Raycast.h"
#include "Physics\Collider.h"
#include "Rendering\Mesh.h"
#include "Rendering\MeshInstance.h"

ToolsideGameObject::ToolsideGameObject(unsigned int guid, string name, GameObjectBase* parent)
 : GameObjectBase(guid, name, parent)
{

}

ToolsideGameObject* ToolsideGameObject::GetChild(int index)
{
    if (index < 0 || index >= (int)m_children.size())
        return NULL;

    return (ToolsideGameObject*)(m_children[index]);
}

int ToolsideGameObject::GetChildNumber()
{
    // determine the index of this game object in the parent's list of children
    if (m_parent)
    {
        vector<GameObjectBase*> children = m_parent->GetChildren();
        std::vector<GameObjectBase*>::iterator iter = std::find(
            children.begin(),
            children.end(),
            this);
        return std::distance(children.begin(), iter);
    }

    return 0;
}

int ToolsideGameObject::GetChildCount()
{
    return m_children.size();
}

bool ToolsideGameObject::InsertChildren(int position, int count)
{
    if (position < 0)
        return false;

    for (int i = 0; i < count; i++)
    {
        unsigned int guid = MakeGuid("GameObject");
        ToolsideGameObject* child = new ToolsideGameObject(guid, "GameObject", NULL);
        child->SetParent(this, position);
    }

    return true;
}

bool ToolsideGameObject::InsertChild(int position, ToolsideGameObject* child)
{
    if (child == NULL || position < 0)
        return false;

    child->SetParent(this, position);
    return true;
}

bool ToolsideGameObject::RemoveChildren(int position, int count)
{
    if (position < 0 || position + count >(int)m_children.size())
        return false;

    m_children.erase(m_children.begin() + position, m_children.begin() + position + count);

    return true;
}


void ToolsideGameObject::AddComponent(ToolsideGameComponent* component)
{
    if (component)
    {
        m_components.push_back(component);
        component->SetGameObject(this);
    }
}

void ToolsideGameObject::RemoveComponent(ToolsideGameComponent* component)
{
    m_components.erase(
        std::remove(m_components.begin(), m_components.end(), component),
        m_components.end());
}

vector<ToolsideGameComponent*>& ToolsideGameObject::GetComponentList()
{
    return m_components;
}

ToolsideGameObject* ToolsideGameObject::DeepCopy(ToolsideGameObject* parent)
{
    unsigned int guid = MakeGuid(m_name);
    ToolsideGameObject* newGO = new ToolsideGameObject(guid, m_name, parent);
    newGO->GetTransform().SetLocalMatrix(GetTransform().GetLocalMatrix());

    if (m_mesh != NULL)
    {
        newGO->SetMeshInstance(m_mesh->DeepCopy());
    }

    // TODO copy other components

    std::vector<GameObjectBase*>::iterator childIter;
    for (childIter = m_children.begin(); childIter != m_children.end(); childIter++)
    {
        ToolsideGameObject* child = (ToolsideGameObject*)*childIter;
        child->DeepCopy(newGO);
    }

    return newGO;
}

// TODO this shouldn't be here
void ToolsideGameObject::Render(bool dirty, bool wireframe)
{
    wireframe |= m_selected;

    // Render the mesh
    if (m_mesh)
    {
        m_mesh->Render(m_transform, wireframe);
    }

    // Render children
    std::vector<GameObjectBase*>::iterator childIter;
    for (childIter = m_children.begin(); childIter != m_children.end(); childIter++)
    {
        ToolsideGameObject* child = (ToolsideGameObject*)*childIter;
        child->Render(dirty, wireframe);
    }

    if (m_selected)
    {
        // Render colliders
        vector<Collider*>::iterator iter;
        for (iter = m_colliders.begin(); iter != m_colliders.end(); iter++)
        {
            (*iter)->DebugDraw(ColorRGB(0.f, 1.f, 0.5f));
        }
    }
}

void ToolsideGameObject::SetSelected(bool selected)
{
    m_selected = selected;
}