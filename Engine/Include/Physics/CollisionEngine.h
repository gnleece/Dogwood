#pragma once

#include "BoundingSphere.h"
#include "BVHNode.h"
#include "Physics/CollisionDetection.h"
#include "Rendering/Color.h"
#include <set>
#include <vector>

#define MAX_POTENTIAL_CONTACTS 25

using std::vector;

class Collider;
class GameObject;

struct PotentialContact
{
    PotentialContact();

    Collider* colliders[2];
};

struct CollisionPair
{
    CollisionPair();
    CollisionPair(GameObject* a, GameObject* b);
    GameObject* gameObjects[2];
};

class CollisionEngine
{
public:

    static CollisionEngine& Singleton()
    {
        static CollisionEngine singleton;
        return singleton;
    }
    CollisionEngine();

    void    Startup();
    void    Shutdown();

    void    CalculateCollisions(float deltaTime);
    void    DrawDebugInfo();
    const   CollisionData* GetCollisionData();

    void    RegisterCollider(Collider* collider);
    void    UnregisterCollider(Collider* collider);

    void    EnableDebugLog(bool enable);
    void    EnableDebugDraw(bool enable);

private:
    void    AddColliderToHierarchy(Collider* collider);
    void    RemoveColliderFromHierarchy(Collider* collider);

    int     BroadPhaseCollision(PotentialContact* potentialContacts);
    int     NarrowPhaseCollision(PotentialContact* potentialContacts, int count, CollisionData* collisionData);

    void    DrawColliders(vector<Collider*>& colliders, ColorRGB color);
    void    DrawBoundingSpheres(BVHNode<BoundingSphere>* bvhNode, ColorRGB color);

    BVHNode<BoundingSphere>*    m_staticCollisionHierarchy;
    vector<Collider*>           m_staticColliders;
    vector<Collider*>           m_dynamicColliders;

    CollisionData               m_collisionData;
    vector<CollisionPair>       m_prevCollisionPairs;

    bool                        m_debugLog;
    bool                        m_debugDraw;
};