#include "Physics/RigidBodyContact.h"
#include "Physics/RigidBody.h"
#include <assert.h>

void RigidBodyContact::CalculateInternals(float deltaTime)
{
    // Check if the first body is NULL, and swap it with the second if it is
    if (Body[0] == NULL)
    {
        SwapBodies();
    }
    assert(Body[0] != NULL);

    // Calculate a set of axes at the contact point
    CalculateContactBasis();

    // Calculate the relative position of the contact relative to each body
    m_relativeContactPosition[0] = ContactPoint - Body[0]->GetPosition();
    if (Body[1])
    {
        m_relativeContactPosition[1] = ContactPoint - Body[1]->GetPosition();
    }

    // Find the relative velocity of the bodies at the contact point
    m_contactVelocity = CalculateLocalVelocity(Body[0], deltaTime, m_relativeContactPosition[0]);
    if (Body[1])
    {
        m_contactVelocity -= CalculateLocalVelocity(Body[1], deltaTime, m_relativeContactPosition[1]);
    }

    // Calculate the desired change in velocity for resolution
    CalculateDesiredDeltaVelocity(deltaTime);
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

Vector3 RigidBodyContact::CalculateLocalVelocity(RigidBody* body, float deltaTime, Vector3 relativeContactPosition)
{
    // Calculate velocity of the contact point
    Vector3 velocity = body->GetAngularVelocity().Cross(relativeContactPosition);
    velocity += body->GetVelocity();

    // Convert velocity into contact coordinates
    // Contact-to-world is rotation-only, so transpose is its inverse (world-to-contact)
    Vector3 velocityContactSpace = m_contactToWorld.Transpose() * velocity;

    // Calculate the amount of velocity that is due to forces without reactions
    //Vector3 accVelocity = body->GetLastFrameAcceleration() * deltaTime;

    return velocityContactSpace;
}

void RigidBodyContact::CalculateDesiredDeltaVelocity(float deltaTime)
{
    // TODO implement me
}

RigidBodyContact::ResolutionResult RigidBodyContact::Resolve(float deltaTime)
{
    ResolveVelocity(deltaTime);
    ResolutionResult result = ResolveInterpenetration(deltaTime);
    return result;
}

// Separating velocity (v_s) = (relative velocity) dot (contact normal) = (v_a - v_b) dot (norm(p_a - p_b))
float RigidBodyContact::CalculateSeparatingVelocity()
{
    Vector3 relativeVelocity = Body[0]->GetVelocity();
    if (Body[1] != NULL)
    {
        relativeVelocity -= Body[1]->GetVelocity();
    }
    return relativeVelocity.Dot(ContactNormal);
}

void RigidBodyContact::CalculateFrictionlessImpulse(Matrix3x3* inverseInertiaTensor)
{
    float deltaVelocity = 0;
    for (int i = 0; i < 2; i ++)
    {
        if (Body[i] != NULL)
        {
            // Calculate a vector that shows the change in velocity in world space for a unit impulse
            // in the direction of the contact normal
            Vector3 deltaVelocityWorldspace = m_relativeContactPosition[i].Cross(ContactNormal);
            deltaVelocityWorldspace = inverseInertiaTensor[i] * deltaVelocityWorldspace;
            deltaVelocityWorldspace = deltaVelocityWorldspace.Cross(m_relativeContactPosition[i]);

            // Calculate the change in velocity in contact coordinates
            deltaVelocity += deltaVelocityWorldspace.Dot(ContactNormal);

            // Add the linear component of velocity change
            deltaVelocity += Body[i]->GetInverseMass();
        }
    }
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
    Vector3 accCausedVelocity = Body[0]->GetAcceleration();
    if (Body[1] != NULL)
    {
        accCausedVelocity -= Body[1]->GetAcceleration();
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
    float totalInverseMass = Body[0]->GetInverseMass();
    if (Body[1] != NULL)
    {
        totalInverseMass += Body[1]->GetInverseMass();
    }

    if (totalInverseMass <= 0)
    {
        // Both bodies have zero inverse mass (i.e. infinite mass) => impulse has no effect
        return;
    }

    // Calculate the amount of impulse per unit of inverse mass
    // We apply the change in velocity in proportion to the inverse mass of each object
    // (Lower inverse mass == higher mass => lower change in velocity)
    float impulse = deltaVelocity / totalInverseMass;
    Vector3 impulsePerInverseMass = ContactNormal * impulse;

    // Apply impulses, in the direction of the contact
    Vector3 velocityA = Body[0]->GetVelocity() + impulsePerInverseMass * Body[0]->GetInverseMass();
    Body[0]->SetVelocity(velocityA);
    if (Body[1] != NULL)
    {
        // Body[1] has motion in the opposite direction from Body[0]
        Vector3 velocityB = Body[1]->GetVelocity() - impulsePerInverseMass * Body[1]->GetInverseMass();
        Body[1]->SetVelocity(velocityB);
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
    float totalInverseMass = Body[0]->GetInverseMass();
    if (Body[1] != NULL)
    {
        totalInverseMass += Body[1]->GetInverseMass();
    }

    if (totalInverseMass <= 0)
    {
        // Both bodies have zero inverse mass (i.e. infinite mass) => no resolution required
        return ResolutionResult();
    }

    // Calculate the movement amounts
    Vector3 movePerInverseMass = ContactNormal * (Penetration / totalInverseMass);
    ResolutionResult result;
    result.MovementA = movePerInverseMass * Body[0]->GetInverseMass();
    result.MovementB = Vector3::Zero;
    if (Body[1] != NULL)
    {
        result.MovementB = movePerInverseMass * -Body[1]->GetInverseMass();
    }

    // Apply penetration resolution
    Vector3 positionA = Body[0]->GetPosition() + result.MovementA;
    Body[0]->SetPosition(positionA);
    if (Body[1] != NULL)
    {
        Vector3 positionB = Body[1]->GetPosition() + result.MovementB;
        Body[1]->SetPosition(positionB);
    }

    return result;
}

void RigidBodyContact::ApplyPositionChange(Vector3* linearChange, Vector3* angularChange, float penetration)
{
    // Calculate inertia of each object in the direction of the contact normal due to angular inertia only
    float linearInertia[2];
    float angularInteria[2];
    float totalInertia;

    Matrix3x3 inverseInertiaTensor = Body[0]->GetInverseIntertiaTensorWorld();

    // Calculate the angular inertia using the same process as for calculating frictionless velocity change
    for (int i = 0; i < 2; i ++)
    {
        if (Body[i] != NULL)
        {
            Vector3 angularInertiaWorld = m_relativeContactPosition[i].Cross(ContactNormal);
            angularInertiaWorld = inverseInertiaTensor*angularInertiaWorld;
            angularInertiaWorld = angularInertiaWorld.Cross(m_relativeContactPosition[i]);
            angularInteria[i] = angularInertiaWorld.Dot(ContactNormal);
        }
    }

    // The linear inertia component is just the inverse mass
    linearInertia[0] = Body[0]->GetInverseMass();
    if (Body[1] != NULL)
    {
        linearInertia[1] = Body[1]->GetInverseMass();
    }

    // Combine angular and linear to calculate total
    totalInertia = linearInertia[0] + angularInteria[0];
    if (Body[1] != NULL)
    {
        totalInertia += linearInertia[1] + angularInteria[1];
    }

    // TODO finish implementation from textbook
}

void RigidBodyContact::SwapBodies()
{
    ContactNormal *= -1;

    RigidBody* temp = Body[0];
    Body[0] = Body[1];
    Body[1] = temp;
}

ContactResolver::ContactResolver(unsigned int maxIterations)
{
    SetMaxIterations(maxIterations);
}

void ContactResolver::SetMaxIterations(unsigned int maxIterations)
{
    m_maxIterations = maxIterations;
}


void ContactResolver::ResolveContacts(RigidBodyContact* contacts, unsigned int numContacts, float deltaTime)
{
    if (numContacts == 0)
        return;

    PrepareContacts(contacts, numContacts, deltaTime);
    AdjustPositions(contacts, numContacts, deltaTime);
    AdjustVelocities(contacts, numContacts, deltaTime);
}

void ContactResolver::PrepareContacts(RigidBodyContact* contacts, unsigned int numContacts, float deltaTime)
{
    // Set up contacts to be ready for processing. This ensures that their internal data is configured
    // correctly and the correct set of bodies is made alive

    // Generate contact velocity and axis information
    RigidBodyContact* lastContact = contacts + numContacts;
    for (RigidBodyContact* contact = contacts; contact < lastContact; contact++)
    {
        contact->CalculateInternals(deltaTime);
    }
}

void ContactResolver::AdjustPositions(RigidBodyContact* contacts, unsigned int numContacts, float deltaTime)
{
    Vector3 linearChange[2], angularChange[2];
    Vector3 deltaPosition;

    // Iteratively resolve interpenetrations in order of severity.
    // The first resolution might change the position of other contacts, so check all contacts again after
    // each iteration. This could end up creating an infinite loop, so stop after m_maxIterations.
    // NOTE / TODO: If objects appear to gradually sink into surfaces and then suddenly jump out after
    // reaching a certain depth, we may need an additional first pass here that considers *all* 
    // interpenetrations (so that shallow ones are skipped by only considering deep ones and then
    // running out of iterations before the shallow ones are reached)

    unsigned int iterationsUsed = 0;
    while (iterationsUsed < m_maxIterations)
    {
        // Find the maximum penetration
        float maxPenetration = FLT_MIN;
        unsigned int maxIndex = numContacts;
        for (unsigned int i = 0; i < numContacts; i++)
        {
            if (contacts[i].Penetration > maxPenetration)
            {
                maxPenetration = contacts[i].Penetration;
                maxIndex = i;
            }
        }
        if (maxIndex == numContacts) break;

        // Match the awake state at the contact     // TODO missing some implementation here
        //contacts[maxIndex].matchAwakeState();

        // Resolve the penetration
        contacts[maxIndex].ApplyPositionChange(linearChange, angularChange, maxPenetration);

        // The resolution may have changed the penetration of other bodies, so update all contacts
        for (unsigned int i = 0; i < numContacts; i++)
        {
            // Check each body in the contact
            for (unsigned int a = 0; a < 2; a++)
            {
                if (contacts[i].Body[a])
                {
                    // Check for a match with each body in the contact that was just resolved
                    for (unsigned int b = 0; b < 2; b++)
                    {
                        if (contacts[i].Body[a] == contacts[maxIndex].Body[b])
                        {
                            Vector3 deltaPosition = linearChange[b] +
                                angularChange[b].Cross(contacts[i].m_relativeContactPosition[a]);

                            // The sign of the change is positive if we're dealing with the
                            // second body in a contact, and negative otherwise
                            contacts[i].Penetration += deltaPosition.Dot(contacts[i].ContactNormal) * (a ? 1 : -1);
                        }
                    }
                }
            }
        }

        iterationsUsed++;
    }
}

void ContactResolver::AdjustVelocities(RigidBodyContact* contacts, unsigned int numContacts, float deltaTime)
{
    // TODO implement me
}