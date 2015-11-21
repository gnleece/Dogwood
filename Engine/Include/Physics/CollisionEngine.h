#pragma once

#include "BoundingSphere.h"
#include "BVHNode.h"
#include <vector>

using std::vector;

class Collider;


struct PotentialContact
{
    Collider* colliders[2];
};

class CollisionEngine
{
public:

    static CollisionEngine& Singleton()
    {
        static CollisionEngine singleton;
        return singleton;
    }
    CollisionEngine() {}

    void    Startup();
    void    Shutdown();

    void    Update(float deltaTime);

    void    RegisterCollider(Collider* collider, bool isStatic = true);
    void    UnregisterCollider(Collider* collider, bool isStatic = true);

private:
    void    BroadPhaseCollision();

    void    InsertStaticColliderInHierarchy(Collider* collider);

    BVHNode<BoundingSphere>*    m_staticCollisionHierarchy;
    vector<Collider*>           m_dynamicColliders;
};