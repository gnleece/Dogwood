#include "Physics/CollisionEngine.h"

#include "GameObject.h"
#include "GameObjectBase.h"
#include "Debugging/DebugDraw.h"
#include "Math/Transformations.h"
#include "Physics/Collider.h"

#include <algorithm>

PotentialContact::PotentialContact()
{
    colliders[0] = NULL;
    colliders[1] = NULL;
}

CollisionEngine::CollisionEngine()
    : m_debugLog(true), m_debugDraw(false), m_collisionData(MAX_POTENTIAL_CONTACTS)
{}

void CollisionEngine::Startup()
{
    m_staticCollisionHierarchy = NULL;
}

void CollisionEngine::Shutdown()
{
    // TODO clean up the collision hierarchy
}

void CollisionEngine::CalculateCollisionData(float deltaTime)
{
    m_collisionData.Reset();

    // Broad phase: generate potential contacts
    PotentialContact potentialContacts[MAX_POTENTIAL_CONTACTS];
    int numPotentialContacts = BroadPhaseCollision(potentialContacts);

    if (m_debugLog)
    {
        if (numPotentialContacts > 0)
        {
            printf("\n\nPotential Contacts\n");
            for (int i = 0; i < numPotentialContacts; i++)
            {
                printf("\t%s\n", potentialContacts[i].colliders[0]->GetGameObject()->GetName().c_str());
                printf("\t%s\n", potentialContacts[i].colliders[1]->GetGameObject()->GetName().c_str());
                printf("\t---\n");
            }
        }
    }

    // Narrow phase: calculate actual contacts
    int numContacts = NarrowPhaseCollision(potentialContacts, numPotentialContacts, &m_collisionData);

    if (numContacts > 0)
    {
        if (m_debugLog) printf("\nActual Contacts\n");
        for (int i = 0; i < numContacts; i++)
        {
            // TODO track enter/exit
            // TODO pass collision info
            ((GameObject*)(m_collisionData.Contacts[i].ColliderA->GetGameObject()))->OnCollisionHold();
            ((GameObject*)(m_collisionData.Contacts[i].ColliderB->GetGameObject()))->OnCollisionHold();

            if (m_debugLog)
            {
                printf("\t%s\n", m_collisionData.Contacts[i].ColliderA->GetGameObject()->GetName().c_str());
                printf("\t%s\n", m_collisionData.Contacts[i].ColliderB->GetGameObject()->GetName().c_str());
                printf("\t---\n");
            }
        }
    }
}

void CollisionEngine::DrawDebugInfo()
{
    if (m_debugDraw)
    {
        DrawBoundingSpheres(m_staticCollisionHierarchy, ColorRGB::White);

        DrawColliders(m_staticColliders, ColorRGB(0.f, 1.f, 0.5f));
        DrawColliders(m_dynamicColliders, ColorRGB::Yellow);
    }
}

const CollisionData* CollisionEngine::GetCollisionData()
{
    return &m_collisionData;
}

void CollisionEngine::DrawColliders(vector<Collider*>& colliders, ColorRGB color)
{
    vector<Collider*>::iterator iter = colliders.begin();
    for (iter = colliders.begin(); iter != colliders.end(); iter++)
    {
        (*iter)->DebugDraw(color);
    }
}

void CollisionEngine::DrawBoundingSpheres(BVHNode<BoundingSphere>* bvhNode, ColorRGB color)
{
    if (bvhNode == NULL)
        return;

    if (bvhNode->GetCollider() != NULL)
    {
        Vector3 position = bvhNode->GetVolume().Center;
        float radius = bvhNode->GetVolume().Radius;
        Matrix4x4 sphereMatrix = Translation(position);
        sphereMatrix = sphereMatrix * UniformScaling(radius);
        DebugDraw::Singleton().DrawSphere(sphereMatrix, color);
    }
    else
    {
        DrawBoundingSpheres(bvhNode->GetChild(0), color);
        DrawBoundingSpheres(bvhNode->GetChild(1), color);
    }
}

void CollisionEngine::RegisterCollider(Collider* collider)
{
    if (collider == NULL)
        return;

    if (collider->IsStatic())
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

    if (collider->IsStatic())
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

void CollisionEngine::EnableDebugLog(bool enable)
{
    m_debugLog = enable;
}

void CollisionEngine::EnableDebugDraw(bool enable)
{
    m_debugDraw = enable;
}

void CollisionEngine::AddColliderToHierarchy(Collider* collider)
{
    if (collider == NULL)
        return;

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
    if (m_staticCollisionHierarchy == NULL)
        return;

    BVHNode<BoundingSphere>* node = m_staticCollisionHierarchy->Find(collider);
    if (node != NULL)
    {
        delete node;    // deleting the node will take care of adjusting the hierarchy
    }
}

int CollisionEngine::BroadPhaseCollision(PotentialContact* potentialContacts)
{
    // Add the dynamic colliders to the collision hierarchy
    vector<Collider*>::iterator iter = m_dynamicColliders.begin();
    for (; iter != m_dynamicColliders.end(); iter++)
    {
        AddColliderToHierarchy(*iter);
    }

    // If there are no colliders at all, there can be no contact
    if (m_staticCollisionHierarchy == NULL)
        return 0;

    // Generate the list of potential contacts
    int numPotentialContacts = m_staticCollisionHierarchy->GetPotentialContacts(potentialContacts, MAX_POTENTIAL_CONTACTS);

    // Remove the dynamic colliders from the hierarchy (because their positions may be different next frame)
    iter = m_dynamicColliders.begin();
    for (; iter != m_dynamicColliders.end(); iter++)
    {
        RemoveColliderFromHierarchy(*iter);
    }

    return numPotentialContacts;
}

int CollisionEngine::NarrowPhaseCollision(PotentialContact* potentialContacts, int count, CollisionData* collisionData)
{
    int numContacts = 0;
    for (int i = 0; i < count; i++)
    {
        Collider* colliderA = potentialContacts[i].colliders[0];
        Collider* colliderB = potentialContacts[i].colliders[1];

        switch (colliderA->GetType())
        {
        case Collider::SPHERE_COLLIDER:
        {
            switch (colliderB->GetType())
            {
            case Collider::SPHERE_COLLIDER:
                numContacts += CollisionDetection::SphereAndSphere((SphereCollider*)colliderA, (SphereCollider*)colliderB, collisionData);
                break;
            case Collider::BOX_COLLIDER:
                numContacts += CollisionDetection::SphereAndBox((SphereCollider*)colliderA, (BoxCollider*)colliderB, collisionData);
                break;
            }
            break;
        }
        case Collider::BOX_COLLIDER:
        {
            switch (colliderB->GetType())
            {
            case Collider::SPHERE_COLLIDER:
                numContacts += CollisionDetection::SphereAndBox((SphereCollider*)colliderB, (BoxCollider*)colliderA, collisionData);
                break;
            case Collider::BOX_COLLIDER:
                numContacts += CollisionDetection::BoxAndBox((BoxCollider*)colliderA, (BoxCollider*)colliderB, collisionData);
            }
            break;
        }
        case Collider::CAPSULE_COLLIDER:
        {
            break;
        }
        }
    }
    return numContacts;
}