#include "Physics\CollisionDetection.h"

CollisionData::CollisionData(int maxContacts)
{
    Contacts = new CollisionContact[maxContacts];
    ContactsRemaining = maxContacts;
}

CollisionData::~CollisionData()
{
    delete[] Contacts;
};

unsigned int CollisionDetection::SphereAndSphere(SphereCollider* a, SphereCollider* b, CollisionData* data)
{
    return 0;
}