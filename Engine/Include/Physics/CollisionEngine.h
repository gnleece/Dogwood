#pragma once

#include "BoundingSphere.h"
#include "BVHNode.h"
#include "Rendering/Color.h"
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
    void    DrawDebugInfo();

    void    RegisterCollider(Collider* collider);
    void    UnregisterCollider(Collider* collider);

private:
    void    AddColliderToHierarchy(Collider* collider);
    void    RemoveColliderFromHierarchy(Collider* collider);

    int     BroadPhaseCollision(PotentialContact* potentialContacts);
    void    NarrowPhaseCollision(PotentialContact* potentialContacts);

    void    DrawColliders(vector<Collider*>& colliders, ColorRGB color);
    void    DrawBoundingSpheres(BVHNode<BoundingSphere>* bvhNode, ColorRGB color);

    BVHNode<BoundingSphere>*    m_staticCollisionHierarchy;
    vector<Collider*>           m_staticColliders;
    vector<Collider*>           m_dynamicColliders;
};