#include "Physics/CollisionEngine.h"

#include <algorithm>


void CollisionEngine::Startup()
{
    m_staticCollisionHierarchy = NULL;
}

void CollisionEngine::Shutdown()
{

}

void CollisionEngine::Update(float deltaTime)
{
    BroadPhaseCollision();
}

void CollisionEngine::RegisterCollider(Collider* collider, bool isStatic)
{
    if (isStatic)
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
    else
    {
        m_dynamicColliders.push_back(collider);
    }
}

void CollisionEngine::UnregisterCollider(Collider* collider, bool isStatic)
{
    if (isStatic)
    {
        BVHNode<BoundingSphere>* node = m_staticCollisionHierarchy->Find(collider);
        if (node != NULL)
        {
            delete node;    // deleting the node will take care of adjusting the hierarchy
        }
    }
    else
    {
        m_dynamicColliders.erase(
            std::remove(m_dynamicColliders.begin(), m_dynamicColliders.end(), collider),
            m_dynamicColliders.end());
    }
}

void CollisionEngine::BroadPhaseCollision()
{

}

void CollisionEngine::InsertStaticColliderInHierarchy(Collider* collider)
{

}