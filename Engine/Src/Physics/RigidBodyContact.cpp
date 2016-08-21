#include "Physics/RigidBodyContact.h"
#include "Physics/RigidBody.h"

RigidBodyContact::ResolutionResult RigidBodyContact::Resolve(float deltaTime)
{
    ResolveVelocity(deltaTime);
    ResolutionResult result = ResolveInterpenetration(deltaTime);
    return result;
}

// Separating velocity (v_s) = (relative velocity) dot (contact normal) = (v_a - v_b) dot (norm(p_a - p_b))
float RigidBodyContact::CalculateSeparatingVelocity()
{
    Vector3 relativeVelocity = BodyA->GetVelocity();
    if (BodyB != NULL)
    {
        relativeVelocity -= BodyB->GetVelocity();
    }
    return relativeVelocity.Dot(ContactNormal);
}

void RigidBodyContact::CalculateContactBasis()
{
    // Calculate an orthonormal basis for the contact space. Choose the contact normal as the
    // x direction, then choose an arbitrary (temp) second direction (either the world x-axis or
    // world y-axis, whichever is further from the contact normal). The cross these two to get the
    // third direction, then cross the third and the first to generate the actual second direction.
    // Code is simplified/optimized (instead of using actual Vector3 cross function).

    Vector3 contactTangents[2];

    if (abs(ContactNormal.x()) > abs(ContactNormal.y()))
    {
        // Scaling factor used for normalization
        float s = 1.0f / sqrtf(ContactNormal.z()*ContactNormal.z() +
                               ContactNormal.x()*ContactNormal.x());

        // The new x-axis is at right angles to the world y-axis
        contactTangents[0].SetX(ContactNormal.z()*s);
        contactTangents[0].SetY(0);
        contactTangents[0].SetZ(-ContactNormal.x()*s);

        // The new y-axis is at right angles to the new x and z-axes
        contactTangents[1].SetX(ContactNormal.y()*contactTangents[0].x());
        contactTangents[1].SetY(ContactNormal.z()*contactTangents[0].x() -
                                ContactNormal.x()*contactTangents[0].z());
        contactTangents[1].SetZ(-ContactNormal.y()*contactTangents[0].x());
    }
    else
    {
        // Scaling factor used for normalization
        float s = 1.0f / sqrtf(ContactNormal.z()*ContactNormal.z() +
            ContactNormal.y()*ContactNormal.y());

        // The new x-axis is at right angles to the world x-axis
        contactTangents[0].SetX(0);
        contactTangents[0].SetY(-ContactNormal.z()*s);
        contactTangents[0].SetZ(ContactNormal.y()*s);

        // The new y-axis is at right angles to the new x and z-axes
        contactTangents[1].SetX(ContactNormal.y()*contactTangents[0].z() -
                                ContactNormal.z()*contactTangents[0].y());
        contactTangents[1].SetY(-ContactNormal.x()*contactTangents[0].z());
        contactTangents[1].SetZ(ContactNormal.x()*contactTangents[0].y());
    }

    m_contactToWorld.SetColumns(ContactNormal, contactTangents[0], contactTangents[1]);
}

void RigidBodyContact::ResolveVelocity(float deltaTime)
{
    // Calculate the velocity in the direction of the contact
    float separatingVelocity = CalculateSeparatingVelocity();

    if (separatingVelocity > 0)
    {
        // The contact is either separating or stationary, so no impulse is required
        return;
    }

    // Use restitution to calculate the change in velocity
    float newSeparatingVelocity = -Restitution * separatingVelocity;

    // Check the velocity buildup due only to acceleration. This is to prevent issues with resting contacts.
    Vector3 accCausedVelocity = BodyA->GetAcceleration();
    if (BodyB != NULL)
    {
        accCausedVelocity -= BodyB->GetAcceleration();
    }
    float accCausedSeparatingVelocity = accCausedVelocity.Dot(ContactNormal) * deltaTime;

    // If we have a closing velocity due to acceleration buildup, remove it from the new separating velocity
    if (accCausedSeparatingVelocity < 0)
    {
        newSeparatingVelocity += Restitution * accCausedSeparatingVelocity;
        if (newSeparatingVelocity < 0)
        {
            newSeparatingVelocity = 0;
        }
    }

    float deltaVelocity = newSeparatingVelocity - separatingVelocity;

    // Calculate the total inverse mass
    float totalInverseMass = BodyA->GetInverseMass();
    if (BodyB != NULL)
    {
        totalInverseMass += BodyB->GetInverseMass();
    }

    if (totalInverseMass <= 0)
    {
        // Both particles has zero inverse mass == infinite mass => impulse has no effect
        return;
    }

    // Calculate the amount of impulse per unit of inverse mass
    // We apply the change in velocity in proportion to the inverse mass of each object
    // (Lower inverse mass == higher mass => lower change in velocity)
    float impulse = deltaVelocity / totalInverseMass;
    Vector3 impulsePerInverseMass = ContactNormal * impulse;

    // Apply impulses, in the direction of the contact
    Vector3 velocityA = BodyA->GetVelocity() + impulsePerInverseMass * BodyA->GetInverseMass();
    BodyA->SetVelocity(velocityA);
    if (BodyB != NULL)
    {
        // BodyB has motion in the opposite direction from BodyA
        Vector3 velocityB = BodyB->GetVelocity() - impulsePerInverseMass * BodyB->GetInverseMass();
        BodyB->SetVelocity(velocityB);
    }
}

RigidBodyContact::ResolutionResult RigidBodyContact::ResolveInterpenetration(float deltaTime)
{
    if (Penetration <= 0)
    {
        // There's no interpenetration, so no resolution is required
        return ResolutionResult();
    }

    // Calculate the total inverse mass
    float totalInverseMass = BodyA->GetInverseMass();
    if (BodyB != NULL)
    {
        totalInverseMass += BodyB->GetInverseMass();
    }

    if (totalInverseMass <= 0)
    {
        // Both particles has zero inverse mass == infinite mass => no resolution required
        return ResolutionResult();
    }

    // Calculate the movement amounts
    Vector3 movePerInverseMass = ContactNormal * (Penetration / totalInverseMass);
    ResolutionResult result;
    result.MovementA = movePerInverseMass * BodyA->GetInverseMass();
    result.MovementB = Vector3::Zero;
    if (BodyB != NULL)
    {
        result.MovementB = movePerInverseMass * -BodyB->GetInverseMass();
    }

    // Apply penetration resolution
    Vector3 positionA = BodyA->GetPosition() + result.MovementA;
    BodyA->SetPosition(positionA);
    if (BodyB != NULL)
    {
        Vector3 positionB = BodyB->GetPosition() + result.MovementB;
        BodyB->SetPosition(positionB);
    }

    return result;
}

ContactResolver::ContactResolver(unsigned int iterations)
{
    SetIterations(iterations);
}

void ContactResolver::SetIterations(unsigned int iterations)
{
    m_iterations = iterations;
}

/* Resolve contacts one by one, starting with the one with the largest closing velocity. This first resolution
may change the closing velocity for other contacts, so check all contacts again after each resolution. 
Since resolving one contact could create another contact, we could end up with an infinite loop of endless contacts.
So, limit the total number of iterations (in practice this should be enough to resolve most contact sets in a 
believable way)*/
void ContactResolver::ResolveContacts(RigidBodyContact* contacts, unsigned int numContacts, float deltaTime)
{
    unsigned int iterationsUsed = 0;
    while(iterationsUsed < m_iterations)
    {
        // Find the contact with the largest closing velocity (i.e. the smallest separating velocity)
        float max = FLT_MAX;
        unsigned int maxIndex = numContacts;
        for (unsigned int i = 0; i < numContacts; i++)
        {
            float separatingVelocity = contacts[i].CalculateSeparatingVelocity();
            if (separatingVelocity < max && (separatingVelocity < 0 || contacts[i].Penetration > 0))
            {
                max = separatingVelocity;
                maxIndex = i;
            }
        }

        // If we didn't find an index worth resolving, we're done
        if (maxIndex == numContacts)
            break;

        // Resolve the contact we chose
        RigidBodyContact::ResolutionResult result = contacts[maxIndex].Resolve(deltaTime);

        // Update the interpenetration for each particle
        for (unsigned int i = 0; i < numContacts; i++)
        {
            if (contacts[i].BodyA == contacts[maxIndex].BodyA)
            {
                contacts[i].Penetration -= result.MovementA.Dot(contacts[i].ContactNormal);
            }
            else if (contacts[i].BodyA == contacts[maxIndex].BodyB)
            {
                contacts[i].Penetration -= result.MovementB.Dot(contacts[i].ContactNormal);
            }

            if (contacts[i].BodyB == NULL)
                continue;

            if (contacts[i].BodyB == contacts[maxIndex].BodyA)
            {
                contacts[i].Penetration += result.MovementA.Dot(contacts[i].ContactNormal);
            }
            else if (contacts[i].BodyB == contacts[maxIndex].BodyB)
            {
                contacts[i].Penetration += result.MovementB.Dot(contacts[i].ContactNormal);
            }
        }

        iterationsUsed++;
    }
}
