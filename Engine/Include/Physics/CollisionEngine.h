#pragma once

#include "BoundingSphere.h"
#include "BVHNode.h"
#include <vector>

#define MAX_POTENTIAL_CONTACTS 25

using std::vector;

class Collider;

struct PotentialContact
{
    Collider* colliders[2];

    PotentialContact()
    {
        colliders[0] = NULL;
        colliders[1] = NULL;
    }
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
    void    AddColliderToHierarchy(Collider* collider);
    void    RemoveColliderFromHierarchy(Collider* collider);

    void    BroadPhaseCollision(PotentialContact* potentialContacts);
    void    NarrowPhaseCollision(PotentialContact* potentialContacts);

    BVHNode<BoundingSphere>*    m_staticCollisionHierarchy;
    vector<Collider*>           m_dynamicColliders;
};