#include "Physics\CollisionDetection.h"
#include "Physics\Collider.h"

CollisionData::CollisionData(int maxContacts)
{
    Contacts = new CollisionContact[maxContacts];
    NextEmptyContact = Contacts;
    ContactsRemaining = maxContacts;
}

CollisionData::~CollisionData()
{
    delete[] Contacts;
};

CollisionContact* CollisionData::ClaimNextContact()
{
    if (ContactsRemaining <= 0)
    {
        return NULL;
    }

    CollisionContact* ret = NextEmptyContact;
    ContactsRemaining--;
    NextEmptyContact++;
    return ret;

}

unsigned int CollisionDetection::SphereAndSphere(SphereCollider* a, SphereCollider* b, CollisionData* data)
{
    if (data->ContactsRemaining <= 0)
        return 0;

    // Cache world positions & radii
    Vector3 aPos = a->GetWorldPosition();
    Vector3 bPos = b->GetWorldPosition();
    float aRadius = a->GetWorldspaceBoundingRadius();
    float bRadius = b->GetWorldspaceBoundingRadius();

    // See if distance between positions is small enough
    Vector3 midline = aPos - bPos;
    float distance = midline.Magnitude();
    if (distance <= 0.0f || distance >= aRadius + bRadius)
    {
        return 0;
    }

    // There is contact, so add contact data to the list
    CollisionContact* contact = data->ClaimNextContact();
    contact->ContactPoint = aPos - midline * 0.5f;
    contact->ContactNormal = midline * (1.0f / distance);
    contact->ColliderA = a;
    contact->ColliderB = b;

    return 1;
}