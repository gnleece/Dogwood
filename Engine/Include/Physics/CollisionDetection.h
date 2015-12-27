#pragma once

#include "Math/Algebra.h"

class Collider;
class BoxCollider;
class CapsuleCollider;
class SphereCollider;

struct CollisionContact
{
public:
    Collider*           ColliderA;
    Collider*           ColliderB;

    Vector3             ContactPoint;
    Vector3             ContactNormal;
    float               Penetration;
};

struct CollisionData
{
    CollisionData(int maxContacts);
    ~CollisionData();

    CollisionContact*   ClaimNextContact();

    CollisionContact*   Contacts;
    CollisionContact*   NextEmptyContact;
    int                 ContactsRemaining;
};

class CollisionDetection
{
public:
    static unsigned int SphereAndSphere(SphereCollider* a, SphereCollider* b, CollisionData* data);
    static unsigned int SphereAndBox(SphereCollider* s, BoxCollider* b, CollisionData* data);
};