#include "Physics/RigidBodyContact.h"
#include "Physics/RigidBody.h"
#include <assert.h>

const float RigidBodyContact::MIN_VELOCITY_LIMIT = 0.25f;

void RigidBodyContact::SwapBodies()
{
    ContactNormal *= -1;

    RigidBody* temp = Body[0];
    Body[0] = Body[1];
    Body[1] = temp;
}

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
    //Vector3 accVelocity = body->GetLastFrameAcceleration() * deltaTime;       // TODO fixme

    return velocityContactSpace;
}

void RigidBodyContact::CalculateDesiredDeltaVelocity(float deltaTime)
{
    // Calculate velocity from acceleration from previous frame. Removing this reduces jitter (pg 394)
    Vector3 scaledContact = deltaTime * ContactNormal;
    float velocityFromAcceleration = Body[0]->GetPreviousAcceleration().Dot(scaledContact);
    if (Body[1])
    {
        velocityFromAcceleration -= Body[1]->GetPreviousAcceleration().Dot(scaledContact);
    }

    // If the velocity is very low, limit the restitution
    float actualRestituion = Restitution;
    if (abs(m_contactVelocity.x()) < MIN_VELOCITY_LIMIT)
    {
        actualRestituion = 0;
    }

    // Combine the bounce velocity with the removed acceleration velocity
    m_desiredDeltaVelocity = -m_contactVelocity.x() -
        actualRestituion*(m_contactVelocity.x() - velocityFromAcceleration);
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

Vector3 RigidBodyContact::CalculateFrictionImpulse(Matrix3x3* inverseInertiaTensor)
{
    float inverseMass = Body[0]->GetInverseMass();

    // The equivalent of a cross-product in matrices is multiplication by a skew-symmetric matrix.
    // We calculate the matrix for converting between linear and angular quantities.
    Matrix3x3 impulseToTorque;
    impulseToTorque.SetSkewSymmetric(m_relativeContactPosition[0]);

    // Calculate the matrix to convert contact impulse to change in velocity in world coordinates
    Matrix3x3 deltaVelocityWorldspace = impulseToTorque;
    deltaVelocityWorldspace = deltaVelocityWorldspace * inverseInertiaTensor[0];
    deltaVelocityWorldspace = deltaVelocityWorldspace * impulseToTorque;
    deltaVelocityWorldspace *= -1;

    if (Body[1] != NULL)
    {
        impulseToTorque.SetSkewSymmetric(m_relativeContactPosition[1]);

        // Calculate the matrix to convert contact impulse to change in velocity in world coordinates
        Matrix3x3 deltaVelocityWorldspace2 = impulseToTorque;
        deltaVelocityWorldspace2 = deltaVelocityWorldspace2 * inverseInertiaTensor[1];
        deltaVelocityWorldspace2 = deltaVelocityWorldspace2 * impulseToTorque;
        deltaVelocityWorldspace2 *= -1;

        // Add to the total delta velocity and inverse mass
        deltaVelocityWorldspace = deltaVelocityWorldspace + deltaVelocityWorldspace2;
        inverseMass += Body[1]->GetInverseMass();
    }

    // Perform a change of basis to convert into contact coordinates
    Matrix3x3 deltaVelocity = m_contactToWorld.Transpose();
    deltaVelocity = deltaVelocity * deltaVelocityWorldspace;        // TODO multiplication order?
    deltaVelocity = deltaVelocity * m_contactToWorld;

    // Add in the linear velocity change
    deltaVelocity[0][0] += inverseMass;
    deltaVelocity[1][1] += inverseMass;
    deltaVelocity[2][2] += inverseMass;

    // Invert to get the impulse needed per unit velocity
    Matrix3x3 impulseMatrix = deltaVelocity.Inverse();

    // Find the target velocities to kill
    Vector3 velKill(m_desiredDeltaVelocity, -m_contactVelocity.y(), -m_contactVelocity.z());

    // Find the impulse to kill target velocities
    Vector3 impulseContact = impulseMatrix * velKill;

    // Check for exceeding friction
    float planarImpulse = sqrtf(impulseContact.y()*impulseContact.y() + impulseContact.z()*impulseContact.z());
    if (planarImpulse > impulseContact.x() * Friction)
    {
        // We need to use dynamic friction
        impulseContact.SetY(impulseContact.y() / planarImpulse);
        impulseContact.SetZ(impulseContact.z() / planarImpulse);
        impulseContact.SetX(deltaVelocity[0][0] +
            deltaVelocity[0][1] * Friction*impulseContact.y() +
            deltaVelocity[0][2] * Friction*impulseContact.z());
        impulseContact.SetX(m_desiredDeltaVelocity / impulseContact.x());
        impulseContact.SetY(impulseContact.y()*Friction*impulseContact.x());
        impulseContact.SetZ(impulseContact.z()*Friction*impulseContact.x());
    }
    return impulseContact;
}

void RigidBodyContact::ApplyPositionChange(Vector3* linearChange, Vector3* angularChange, float penetration)
{
    float angularLimit = 2.0f;
    float angularMove[2];
    float linearMove[2];
    float linearInertia[2];
    float angularInteria[2];
    float totalInertia = 0.0f;

    // Calculate the angular inertia using the same process as for calculating frictionless velocity change
    for (int i = 0; i < 2; i++)
    {
        if (Body[i] != NULL)
        {
            Matrix3x3 inverseInertiaTensor = Body[0]->GetInverseIntertiaTensorWorld();
            Vector3 angularInertiaWorld = m_relativeContactPosition[i].Cross(ContactNormal);
            angularInertiaWorld = inverseInertiaTensor*angularInertiaWorld;
            angularInertiaWorld = angularInertiaWorld.Cross(m_relativeContactPosition[i]);
            angularInteria[i] = angularInertiaWorld.Dot(ContactNormal);

            // The linear inertia component is just the inverse mass
            linearInertia[i] = Body[i]->GetInverseMass();

            // Combine angular and linear to calculate total
            totalInertia += linearInertia[i] + angularInteria[i];
        }
    }

    // Calculate and apply changes
    for (int i = 0; i < 2; i++)
    {
        if (Body[i] != NULL)
        {
            // The linear and angular movements required are in proportion to the two inverse inertias
            float sign = (i == 0) ? 1.f : -1.f;
            angularMove[i] = sign * penetration * (angularInteria[i] / totalInertia);
            linearMove[i] = sign * penetration * (linearInertia[i] / totalInertia);

            // To avoid angular projections that are too great (when mass is large but inertia
            // tensor is small), limit the angular move
            Vector3 projection = m_relativeContactPosition[i];
            projection += ContactNormal * -m_relativeContactPosition[i].Dot(ContactNormal);

            // Use the small angle approximation for the sine of the angle (i.e. the magnitude would be
            // sin(angularLimit) * projection.magnitude, but we approximate sin(angularLimit) to angularLimit.
            float maxMagnitude = angularLimit * projection.Magnitude();

            if (angularMove[i] < -maxMagnitude)
            {
                float totalMove = angularMove[i] + linearMove[i];
                angularMove[i] = -maxMagnitude;
                linearMove[i] = totalMove - angularMove[i];
            }
            else if (angularMove[i] > maxMagnitude)
            {
                float totalMove = angularMove[i] + linearMove[i];
                angularMove[i] = maxMagnitude;
                linearMove[i] = totalMove - angularMove[i];
            }

            // We have the linear amount of movement required by turning the rigid body (angularMove[i]).
            // We now need to calculate the desired rotation to achieve that.
            if (Approximately(angularMove[i], 0.f))
            {
                // Easy case - no angular movement means no rotation.
                angularChange[i] = Vector3::Zero;
            }
            else
            {
                // Work out the direction we'd like to rotate in
                Vector3 targetAngularDirection = m_relativeContactPosition[i].Cross(ContactNormal);

                // Work out the direction we'd need to rotate to achieve that
                Matrix3x3 inverseInertiaTensor = Body[i]->GetInverseIntertiaTensorWorld();
                angularChange[i] = (inverseInertiaTensor * targetAngularDirection) * (angularMove[i] / angularInteria[i]);
            }

            // Velocity change is just the linear movement along the contact normal
            linearChange[i] = ContactNormal * linearMove[i];

            // Apply the linear movement
            Vector3 pos = Body[i]->GetPosition();
            pos += linearMove[i] * ContactNormal;
            Body[i]->SetPosition(pos);

            // Apply the change in orientation
            Quaternion rot = Body[i]->GetRotation();
            rot.AddScaledVector(angularChange[i], 1.0);
            Body[i]->SetRotation(rot);

            // TODO calculate derived data for bodies that are not awake
        }
    }
}

void RigidBodyContact::ApplyVelocityChange(Vector3* velocityChange, Vector3* angularVelocityChange)
{
    // TODO implement me
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
    Vector3 velocityChange[2], angularVelocityChange[2];
    Vector3 deltaVelocity;

    // Iteratively resolve impacts in order of severity.
    unsigned int iterationsUsed = 0;
    while (iterationsUsed < m_maxIterations)
    {
        // Find the maximum probable velocity change
        float maxVelocity = FLT_MIN;
        unsigned int maxIndex = numContacts;
        for (unsigned int i = 0; i < numContacts; i++)
        {
            if (contacts[i].m_desiredDeltaVelocity > maxVelocity)
            {
                maxVelocity = contacts[i].m_desiredDeltaVelocity;
                maxIndex = i;
            }
        }
        if (maxIndex == numContacts) break;

        // Match the awake state at the contact     // TODO missing some implementation here
        //contacts[maxIndex].matchAwakeState();

        // Apply resolution on selected contact
        contacts[maxIndex].ApplyVelocityChange(velocityChange, angularVelocityChange);

        // The resolution may have changed the velocity of other bodies, so update all contacts
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
                            Vector3 deltaVelocity = velocityChange[b] +
                                angularVelocityChange[b].Cross(contacts[i].m_relativeContactPosition[a]);

                            // The sign of the change is positive if we're dealing with the
                            // second body in a contact, and negative otherwise
                            contacts[i].m_contactVelocity +=
                                contacts[i].m_contactToWorld.Transpose()*deltaVelocity  * (a ? -1 : 1);
                            contacts[i].CalculateDesiredDeltaVelocity(deltaTime);
                        }
                    }
                }
            }
        }

        iterationsUsed++;
    }
}