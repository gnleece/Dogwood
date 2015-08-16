#include "GameObject.h"

#include "Util.h"
#include "Math\Raycast.h"
#include "Rendering\Mesh.h"
#include "Rendering\MeshInstance.h"
#include "GameComponent.h"
#include "GameObjectReference.h"
#include "ToolsideGameComponent.h"

#include <algorithm>

vector<GameObject*> GameObject::ActiveGameObjects = vector<GameObject*>();

GameObject::GameObject(unsigned int guid, string name, GameObject* parent)
 : m_dirty(true), m_selected(false)
{
    m_guid = guid;
    m_name = name;
    SetParent(parent);

    // TODO unregister on destroy
    ActiveGameObjects.push_back(this);
    GameObjectReference::AddToMap(guid, this);
}

Transform& GameObject::GetLocalTransform()
{
    return m_localTransform;
}

Transform GameObject::GetWorldTransform()
{
    // TODO optimize this
    if (m_parent)
    {
        return m_parent->GetWorldTransform()*m_localTransform;
    }

    return m_localTransform;
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

unsigned int GameObject::GetID()
{
    return m_guid;
}

string GameObject::GetName()
{
    return m_name;
}

void GameObject::SetName(string name)
{
    m_name = name;
}

void GameObject::SetName(const char* name)
{
    if (name)
    {
        m_name = name;
    }
}

GameObject* GameObject::GetParent()
{
    return m_parent;
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

    m_dirty = true;
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
    if (index < 0 || index >= (int)m_children.size())
        return NULL;

    return m_children[index];
}

vector<GameObject*>& GameObject::GetChildren()
{
    return m_children;
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
        unsigned int guid = MakeGuid("GameObject");
        GameObject* child = new GameObject(guid, "GameObject", NULL);
        child->SetParent(this, position);
    }

    return true;
}

bool GameObject::InsertChild(int position, GameObject* child)
{
    if (child == NULL || position < 0)
        return false;

    child->SetParent(this, position);
    return true;
}

bool GameObject::RemoveChildren(int position, int count)
{
    if (position < 0 || position + count > (int)m_children.size())
        return false;

    m_children.erase(m_children.begin() + position, m_children.begin() + position + count);

    return true;
}

GameObject* GameObject::DeepCopy(GameObject* parent)
{
    unsigned int guid = MakeGuid(m_name);
    GameObject* newGO = new GameObject(guid, m_name, parent);
    newGO->SetLocalTransform(m_localTransform);

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

void GameObject::Render(Transform& parentWorldTransform, bool dirty, bool wireframe)
{
    // Determine worldspace transform
    dirty |= m_dirty | m_localTransform.HasChanged();
    wireframe |= m_selected;
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

    // Render all components
    std::vector<GameComponent*>::iterator compIter;
    for (compIter = m_components.begin(); compIter != m_components.end(); compIter++)
    {
        GameComponent* component = *compIter;
        component->Render(m_worldTransform, wireframe);
    }

    // Render children
    std::vector<GameObject*>::iterator childIter;
    for (childIter = m_children.begin(); childIter != m_children.end(); childIter++)
    {
        GameObject* child = *childIter;
        child->Render(m_worldTransform, dirty, wireframe);
    }
}

void GameObject::SetMesh(MeshInstance* mesh)
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

MeshInstance* GameObject::GetMesh()
{
    return m_mesh;
}

void GameObject::AddChild(GameObject* child, int index)
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

void GameObject::RemoveChild(GameObject* child)
{
    // TODO fixme
    //m_children.remove(child);
}

void GameObject::SetSelected(bool selected)
{
    m_selected = selected;
}

GameObject* GameObject::BoundingSphereRaycast(Vector3 rayOrigin, Vector3 rayDirection, Transform& parentWorldTransform, float& distance)
{
    // TODO this function is less optimized than Render, i.e. we don't both caching the world transform
    // This is because this function shouldn't get called very often currently (e.g. right now it is only
    // needed for object picking in the Editor). Also, this code probably shouldn't live here. Also, because
    // of the way we apply scale to the radius, the bounding sphere is nowhere near optimal. But it is good enough :)

    // TODO also it's kind of insane to raycast against literally every object in the scene here
    // Really, we should do some kind of space partitioning first

    Transform worldTransform = parentWorldTransform*m_localTransform;
    bool hit = false;
    float bestDistance = FLT_MAX;
    GameObject* bestGameObject = NULL;

    // Raycast against self, if we have a mesh
    if (m_mesh != NULL && m_mesh->GetMesh() != NULL)
    {
        // Calculate the radius for a bounding sphere for the mesh
        float radius = m_mesh->GetMesh()->GetBoundingRadius();
        Vector3 scale = worldTransform.GetScale();
        float maxScale = std::fmaxf(scale[0], scale[1]);
        maxScale = std::fmaxf(maxScale, scale[2]);
        radius *= maxScale;        // this is pretty hacky and gives a suboptimal radius, but it's good enough

        Vector3 objectCenter = worldTransform.GetPosition();

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
    std::vector<GameObject*>::iterator childIter;
    for (childIter = m_children.begin(); childIter != m_children.end(); childIter++)
    {
        GameObject* child = *childIter;
        float childDistance;
        GameObject* hitObject = child->BoundingSphereRaycast(rayOrigin, rayDirection, worldTransform, childDistance);
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

void GameObject::AddToolsideComponent(ToolsideGameComponent* component)
{
    if (component)
    {
        m_toolComponents.push_back(component);
        component->SetGameObject(this);
    }
}

void GameObject::RemoveToolsideComponent(ToolsideGameComponent* component)
{
    m_toolComponents.erase(
        std::remove(m_toolComponents.begin(), m_toolComponents.end(), component), 
        m_toolComponents.end());
}

vector<ToolsideGameComponent*>& GameObject::GetToolsideComponentList()
{
    return m_toolComponents;
}