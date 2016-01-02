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
    contact->Penetration = aRadius + bRadius - distance;
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
    Vector3 closestPointWorldspace = b->GetTransform().TransformPoint(closestPoint);
    float distanceSqrd = (closestPointWorldspace - sphereWorldPos).MagnitudeSqrd();
    if (distanceSqrd > sphereWorldRadius * sphereWorldRadius)
    {
        return 0;
    }

    // The point is close enough and therefore there is a contact. Set contact data
    CollisionContact* contact = data->ClaimNextContact();
    contact->ContactPoint = closestPointWorldspace;
    contact->ContactNormal = (closestPointWorldspace - sphereWorldPos).Normalized();
    contact->Penetration = sphereWorldRadius - sqrt(distanceSqrd);
    contact->ColliderA = s;
    contact->ColliderB = b;

    return 1;
}

unsigned int CollisionDetection::BoxAndBox(BoxCollider* a, BoxCollider* b, CollisionData* data)
{
    // Determine whether the boxes overlap by performing separating axis tests (SATs)
    // In this case (box-and-box) there are 15 axis tests to consider.
    const int NUM_CASES = 15;
    Vector3 axes[NUM_CASES];
    Transform aTrans = a->GetTransform();
    Transform bTrans = b->GetTransform();

    // Face axes for box A
    axes[0] = aTrans.GetRight();
    axes[1] = aTrans.GetUp();
    axes[2] = aTrans.GetForward();

    // Face axes for box B
    axes[3] = bTrans.GetRight();
    axes[4] = bTrans.GetUp();
    axes[5] = bTrans.GetForward();

    // Edge-edge axes
    axes[6] = aTrans.GetRight().Cross(bTrans.GetRight());
    axes[7] = aTrans.GetRight().Cross(bTrans.GetUp());
    axes[8] = aTrans.GetRight().Cross(bTrans.GetForward());
    axes[9] = aTrans.GetUp().Cross(bTrans.GetRight());
    axes[10] = aTrans.GetUp().Cross(bTrans.GetUp());
    axes[11] = aTrans.GetUp().Cross(bTrans.GetForward());
    axes[12] = aTrans.GetForward().Cross(bTrans.GetRight());
    axes[13] = aTrans.GetForward().Cross(bTrans.GetUp());
    axes[14] = aTrans.GetForward().Cross(bTrans.GetForward());

    // Perform test for each axis to find the case with the best overlap
    float bestOverlap = FLT_MAX;
    unsigned int bestCase;
    Vector3 centerAToCenterB = b->GetTransform().GetWorldPosition() - a->GetTransform().GetWorldPosition();
    for (int i = 0; i < NUM_CASES; i++)
    {
        Vector3 axis = axes[i];

        // Check for axes that were generated by (almost) parallel edges
        if (axis.MagnitudeSqrd() < 0.001f)
        {
            continue;
        }

        axis.Normalize();
        float overlap = PenetrationOnAxis(a, b, axis, centerAToCenterB);
        if (overlap < 0)
        {
            // A single case of negative overlap indicates that there can be no collision
            return 0;
        }
        if (overlap < bestOverlap)
        {
            // Overlap is positive, indiating there *may* be a contact
            bestOverlap = overlap;
            bestCase = i;
        }
    }

    // Calculate contact data based on which axis test generated the best overlap
    Vector3 axis = axes[bestCase];
    if (bestCase < 3)
    {
        // Contact between face of box A and vertex of box B
        SetFaceVertexContactData(a, b, centerAToCenterB, data, axis, bestOverlap);
        return 1;
    }
    else if (bestCase < 6)
    {
        // Contact between face of box B and vertex of box A
        SetFaceVertexContactData(b, a, -1.0f*centerAToCenterB, data, axis, bestOverlap);
        return 1;
    }
    else
    {
        // Edge-edge contact. Determine which axis are involved
        bestCase -= 6;
        unsigned int oneAxisIndex = bestCase / 3;
        unsigned int twoAxisIndex = bestCase % 3;

        // Calculate contact data
        SetEdgeEdgeContactData(a, b, data, oneAxisIndex, twoAxisIndex, centerAToCenterB, bestOverlap);
        return 1;
    }

    return 0;
}

float CollisionDetection::ProjectToAxis(BoxCollider* box, Vector3& axis)
{
    Transform transform = box->GetTransform();
    Vector3 halfsize = box->GetWorldScaleHalfsize();

    float projection =  halfsize.x() * abs(axis.Dot(transform.GetRight())) +
                        halfsize.y() * abs(axis.Dot(transform.GetUp())) +
                        halfsize.z() * abs(axis.Dot(transform.GetForward()));

    return projection;
}

float CollisionDetection::PenetrationOnAxis(BoxCollider* a, BoxCollider* b, Vector3& axis, Vector3& centerAToCenterB)
{
    float projectionA = ProjectToAxis(a, axis);
    float projectionB = ProjectToAxis(b, axis);

    float distance = abs(centerAToCenterB.Dot(axis));

    // Calculate overlap: positive value => overlap, negative value => separation
    return projectionA + projectionB - distance;
}

void CollisionDetection::SetFaceVertexContactData(BoxCollider* faceBox, BoxCollider* vertexBox, Vector3& centerToCenter, CollisionData* data, Vector3 axis, float bestOverlap)
{
    // Determine which face is in contact (we know it's one of the two along the given axis)
    if (axis.Dot(centerToCenter) > 0)
    {
        axis = -1.0f * axis;
    }

    // Determine which vertex of the other box is in the contact, in that box's coordinate space
    Vector3 vertex = vertexBox->GetWorldScaleHalfsize();
    if (vertexBox->GetTransform().GetRight().Dot(axis) < 0)
    {
        vertex.SetX(vertex.x());
    }
    if (vertexBox->GetTransform().GetUp().Dot(axis) < 0)
    {
        vertex.SetY(vertex.y());
    }
    if (vertexBox->GetTransform().GetForward().Dot(axis) < 0)
    {
        vertex.SetZ(vertex.z());
    }

    // Set the contact data
    CollisionContact* contact = data->ClaimNextContact();
    contact->ContactPoint = vertexBox->GetTransform().TransformPoint(vertex);
    contact->ContactNormal = axis;
    contact->Penetration = bestOverlap;
    contact->ColliderA = faceBox;
    contact->ColliderB = vertexBox;
}

void CollisionDetection::SetEdgeEdgeContactData(BoxCollider* a, BoxCollider* b, CollisionData* data, int oneAxisIndex, int twoAxisIndex, Vector3& centerToCenter, float bestOverlap)
{
    Vector3 oneAxis = a->GetTransform().GetAxis(oneAxisIndex);
    Vector3 twoAxis = b->GetTransform().GetAxis(twoAxisIndex);
    Vector3 axis = oneAxis.Cross(twoAxis);
    axis.Normalize();

    // The axis should point from box one to box two.
    if (axis.Dot(centerToCenter) > 0)
    {
        axis = -1.0f * axis;
    }

    Vector3 pointOnEdgeOne = a->GetWorldScaleHalfsize();
    Vector3 pointOnEdgeTwo = b->GetWorldScaleHalfsize();

    for (int i = 0; i < 3; i++)
    {
        if (i == oneAxisIndex)
        {
            pointOnEdgeOne[i] = 0;
        }
        else if (a->GetTransform().GetAxis(i).Dot(axis) > 0)
        {
            pointOnEdgeTwo[i] = -pointOnEdgeOne[i];
        }

        if (i == twoAxisIndex)
        {
            pointOnEdgeOne[i] = 0;
        }
        else if (b->GetTransform().GetAxis(i).Dot(axis) < 0)
        {
            pointOnEdgeTwo[i] = -pointOnEdgeTwo[i];
        }
    }

    // Transform points into world space
    pointOnEdgeOne = a->GetTransform().TransformPoint(pointOnEdgeOne);
    pointOnEdgeTwo = b->GetTransform().TransformPoint(pointOnEdgeTwo);

    // Calculate the contact point
    Vector3 vertex = GetContactPoint(oneAxis, twoAxis, pointOnEdgeOne, pointOnEdgeTwo);

    // Fill in contact data
    CollisionContact* contact = data->ClaimNextContact();
    contact->ContactPoint = vertex;
    contact->ContactNormal = axis;
    contact->Penetration = bestOverlap;
    contact->ColliderA = a;
    contact->ColliderB = b;
}

Vector3 CollisionDetection::GetContactPoint(Vector3& axisOne, Vector3& axisTwo, Vector3& pointOnEdgeOne, Vector3& pointOnEdgeTwo)
{
    // Vector between the test points
    Vector3 toSt = pointOnEdgeOne - pointOnEdgeTwo;

    // How much of that vector is in the direction of each edge?
    float dotStaOne = axisOne.Dot(toSt);
    float dotStaTwo = axisTwo.Dot(toSt);

    // Calculate the closest point along each edge
    float sqrMagOne = axisOne.MagnitudeSqrd();
    float sqrMagTwo = axisTwo.MagnitudeSqrd();
    float dotProductEdges = axisTwo.Dot(axisOne);
    float denom = sqrMagOne * sqrMagTwo - dotProductEdges * dotProductEdges;
    float a = (dotProductEdges * dotStaTwo - sqrMagTwo * dotStaOne) / denom;
    float b = (sqrMagOne * dotStaTwo - dotProductEdges * dotStaOne) / denom;

    // Use a point midway between the two closest points as the contact point
    Vector3 nearestPointOne = pointOnEdgeOne + axisOne * a;
    Vector3 nearestPointTwo = pointOnEdgeTwo + axisTwo * b;
    return (nearestPointOne + nearestPointTwo) * 0.5f;
}