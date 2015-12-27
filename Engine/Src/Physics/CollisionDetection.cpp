#include "Physics\CollisionDetection.h"

#include "GameObjectBase.h"
#include "Math\Transformations.h"
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

unsigned int CollisionDetection::SphereAndBox(SphereCollider* s, BoxCollider* b, CollisionData* data)
{
    // If the box has zero scale along any axis, no collision is possible
    Vector3 boxWorldScale = b->GetTransform().GetWorldScale();
    if (boxWorldScale.HasZeroComponent())
    {
        return 0;
    }

    // Cache the local (i.e. box space) scale of the box
    Vector3 boxHalfsize = b->GetLocalSize();

    // Transform the world position of the sphere into the local space of the box
    Vector3 sphereWorldPos = s->GetWorldPosition();
    Vector3 sphereBoxspacePos = b->GetTransform().InverseTransformPoint(sphereWorldPos);

    // Convert the scale (radius) of the sphere into the local space of the box
    float sphereWorldRadius = s->GetWorldspaceBoundingRadius();
    Vector3 sphereBoxspaceScale = sphereWorldRadius * boxWorldScale.ComponentwiseInverse();

    // Early out check to see if we can exclude the contact
    if (abs(sphereBoxspacePos.x()) - sphereBoxspaceScale.x() > boxHalfsize.x() ||
        abs(sphereBoxspacePos.y()) - sphereBoxspaceScale.y() > boxHalfsize.y() ||
        abs(sphereBoxspacePos.z()) - sphereBoxspaceScale.z() > boxHalfsize.z())
    {
        return 0;
    }

    // Calculate the point on the box that is closest to the sphere by clamping each
    // coordinate of the sphere's position onto the box
    Vector3 closestPoint;
    float coordDist = 0.0f;

    coordDist = sphereBoxspacePos.x();
    if (coordDist > boxHalfsize.x()) coordDist = boxHalfsize.x();
    if (coordDist < -boxHalfsize.x()) coordDist = -boxHalfsize.x();
    closestPoint.SetX(coordDist);

    coordDist = sphereBoxspacePos.y();
    if (coordDist > boxHalfsize.y()) coordDist = boxHalfsize.y();
    if (coordDist < -boxHalfsize.y()) coordDist = -boxHalfsize.y();
    closestPoint.SetY(coordDist);

    coordDist = sphereBoxspacePos.z();
    if (coordDist > boxHalfsize.z()) coordDist = boxHalfsize.z();
    if (coordDist < -boxHalfsize.z()) coordDist = -boxHalfsize.z();
    closestPoint.SetZ(coordDist);

    // Calculate the distance between the sphere and the closest point on the box, to see if
    // we are close enough for contact
    float distance = (closestPoint - sphereBoxspacePos).MagnitudeSqrd();
    if (distance > sphereWorldRadius * sphereWorldRadius)
    {
        return 0;
    }

    // The point is close enough and therefore there is a contact. Set contact data
    Vector3 closestPointWorldspace = b->GetTransform().TransformPoint(closestPoint);
    CollisionContact* contact = data->ClaimNextContact();
    contact->ContactPoint = closestPointWorldspace;
    contact->ContactNormal = (closestPointWorldspace - sphereWorldPos).Normalized();
    contact->ColliderA = s;
    contact->ColliderB = b;

    return 1;
}