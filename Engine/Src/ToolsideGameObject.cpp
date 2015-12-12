#include "ToolsideGameObject.h"

#include <algorithm>

#include "ToolsideGameComponent.h"
#include "Util.h"
#include "Math\Raycast.h"
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
        newGO->SetMesh(m_mesh->DeepCopy());
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

ToolsideGameObject* ToolsideGameObject::BoundingSphereRaycast(Vector3 rayOrigin, Vector3 rayDirection, float& distance)
{
    // TODO this function is less optimized than Render, i.e. we don't both caching the world transform
    // This is because this function shouldn't get called very often currently (e.g. right now it is only
    // needed for object picking in the Editor). Also, this code probably shouldn't live here. Also, because
    // of the way we apply scale to the radius, the bounding sphere is nowhere near optimal. But it is good enough :)

    // TODO also it's kind of insane to raycast against literally every object in the scene here
    // Really, we should do some kind of space partitioning first

    bool hit = false;
    float bestDistance = FLT_MAX;
    ToolsideGameObject* bestGameObject = NULL;

    // Raycast against self, if we have a mesh
    if (m_mesh != NULL && m_mesh->GetMesh() != NULL)
    {
        // Calculate the radius for a bounding sphere for the mesh
        float radius = m_mesh->GetMesh()->GetBoundingRadius();
        Vector3 scale = m_transform.GetWorldScale();
        float maxScale = std::fmaxf(scale[0], scale[1]);
        maxScale = std::fmaxf(maxScale, scale[2]);
        radius *= maxScale;        // this is pretty hacky and gives a suboptimal radius, but it's good enough

        Vector3 objectCenter = m_transform.GetWorldPosition();

        // Do the raycast
        Raycast::HitInfo hitInfo;
        hit = Raycast::RaycastBoundingSphere(rayOrigin, rayDirection, radius, objectCenter, hitInfo);
        if (hit)
        {
            bestDistance = hitInfo.distance;
            bestGameObject = this;
        }
    }

    // Raycast against children
    std::vector<GameObjectBase*>::iterator childIter;
    for (childIter = m_children.begin(); childIter != m_children.end(); childIter++)
    {
        ToolsideGameObject* child = (ToolsideGameObject*)*childIter;    // TODO hacky
        float childDistance;
        ToolsideGameObject* hitObject = child->BoundingSphereRaycast(rayOrigin, rayDirection, childDistance);
        if (hitObject != NULL)
        {
            hit = true;
            if (childDistance < bestDistance)
            {
                bestDistance = childDistance;
                bestGameObject = hitObject;
            }
        }
    }

    // Return object with smallest distance from ray origin
    if (hit)
    {
        distance = bestDistance;
        return bestGameObject;
    }

    distance = FLT_MAX;
    return NULL;
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
}

void ToolsideGameObject::SetSelected(bool selected)
{
    m_selected = selected;
}