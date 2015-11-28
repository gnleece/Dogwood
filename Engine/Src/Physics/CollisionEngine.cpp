#include "Physics/CollisionEngine.h"

#include "Debugging/DebugDraw.h"
#include "Math/Transformations.h"
#include "Physics/Collider.h"

#include <algorithm>


void CollisionEngine::Startup()
{
    m_staticCollisionHierarchy = NULL;
}

void CollisionEngine::Shutdown()
{
    // TODO clean up the collision hierarchy
}

void CollisionEngine::Update(float deltaTime)
{
    // Broad phase: generate potential contacts
    PotentialContact potentialContacts[MAX_POTENTIAL_CONTACTS];
    //BroadPhaseCollision(potentialContacts);

    // Narrow phase: calculate actual contacts
    NarrowPhaseCollision(potentialContacts);
}

void CollisionEngine::DrawDebugInfo()
{
    DrawBoundingSpheres(m_staticColliders, ColorRGB::White);
    DrawBoundingSpheres(m_dynamicColliders, ColorRGB::Yellow);
}

void CollisionEngine::DrawBoundingSpheres(vector<Collider*>& colliders, ColorRGB color)
{
    vector<Collider*>::iterator iter = colliders.begin();
    for (iter = colliders.begin(); iter != colliders.end(); iter++)
    {
        Vector3 position = (*iter)->GetTransform().GetWorldPosition();
        float radius = (*iter)->GetBoundingRadius();
        Matrix4x4 sphereMatrix = Translation(position);
        sphereMatrix = sphereMatrix * UniformScaling(radius);
        DebugDraw::Singleton().DrawSphere(sphereMatrix, color);
    }
}

void CollisionEngine::RegisterCollider(Collider* collider)
{
    if (collider == NULL)
        return;

    if (collider->IsStatic)
    {
        m_staticColliders.push_back(collider);
        AddColliderToHierarchy(collider);
    }
    else
    {
        m_dynamicColliders.push_back(collider);
    }
}

void CollisionEngine::UnregisterCollider(Collider* collider)
{
    if (collider == NULL)
        return;

    if (collider->IsStatic)
    {
        m_staticColliders.erase(
            std::remove(m_staticColliders.begin(), m_staticColliders.end(), collider),
            m_staticColliders.end());

        RemoveColliderFromHierarchy(collider);
    }
    else
    {
        m_dynamicColliders.erase(
            std::remove(m_dynamicColliders.begin(), m_dynamicColliders.end(), collider),
            m_dynamicColliders.end());
    }
}

void CollisionEngine::AddColliderToHierarchy(Collider* collider)
{
    BoundingSphere boundingSphere(collider);
    if (m_staticCollisionHierarchy == NULL)
    {
        m_staticCollisionHierarchy = new BVHNode<BoundingSphere>(NULL, collider, boundingSphere);
    }
    else
    {
        m_staticCollisionHierarchy->Insert(collider, boundingSphere);
    }
}

void CollisionEngine::RemoveColliderFromHierarchy(Collider* collider)
{
    BVHNode<BoundingSphere>* node = m_staticCollisionHierarchy->Find(collider);
    if (node != NULL)
    {
        delete node;    // deleting the node will take care of adjusting the hierarchy
    }
}

void CollisionEngine::BroadPhaseCollision(PotentialContact* potentialContacts)
{
    // Add the dynamic colliders to the collision hierarchy
    vector<Collider*>::iterator iter = m_dynamicColliders.begin();
    for (; iter != m_dynamicColliders.end(); iter++)
    {
        AddColliderToHierarchy(*iter);
    }

    // Generate the list of potential contacts
    m_staticCollisionHierarchy->GetPotentialContacts(potentialContacts, MAX_POTENTIAL_CONTACTS);

    // Remove the dynamic colliders from the hierarchy (because their positions may be different next frame)
    iter = m_dynamicColliders.begin();
    for (; iter != m_dynamicColliders.end(); iter++)
    {
        RemoveColliderFromHierarchy(*iter);
    }
}

void CollisionEngine::NarrowPhaseCollision(PotentialContact* potentialContacts)
{

}

