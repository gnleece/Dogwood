#include "Physics/Particles/ParticleContact.h"
#include "Physics/Particles/PhysicsParticle.h"

void ParticleContact::Resolve(float duration)
{
    ResolveVelocity(duration);
    ResolveInterpenetration(duration);
}

// Separating velocity (v_s) = (relative velocity) dot (contact normal) = (v_a - v_b) dot (norm(p_a - p_b))
float ParticleContact::CalculateSeparatingVelocity()
{
    Vector3 relativeVelocity = ParticleA->GetVelocity();
    if (ParticleB != NULL)
    {
        relativeVelocity -= ParticleB->GetVelocity();
    }
    return relativeVelocity.Dot(ContactNormal);
}

void ParticleContact::ResolveVelocity(float duration)
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
    Vector3 accCausedVelocity = ParticleA->GetAcceleration();
    if (ParticleB != NULL)
    {
        accCausedVelocity -= ParticleB->GetAcceleration();
    }
    float accCausedSeparatingVelocity = accCausedVelocity.Dot(ContactNormal) * duration;

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
    float totalInverseMass = ParticleA->GetInverseMass();
    if (ParticleB != NULL)
    {
        totalInverseMass += ParticleB->GetInverseMass();
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
    Vector3 velocityA = ParticleA->GetVelocity() + impulsePerInverseMass * ParticleA->GetInverseMass();
    ParticleA->SetVelocity(velocityA);
    if (ParticleB != NULL)
    {
        // ParticleB has motion in the opposite direction from ParticleA
        Vector3 velocityB = ParticleB->GetVelocity() - impulsePerInverseMass * ParticleB->GetInverseMass();
        ParticleB->SetVelocity(velocityB);
    }
}

void ParticleContact::ResolveInterpenetration(float duration)
{
    if (Penetration <= 0)
    {
        // There's no interpenetration, so no resolution is required
        return;
    }

    // Calculate the total inverse mass
    float totalInverseMass = ParticleA->GetInverseMass();
    if (ParticleB != NULL)
    {
        totalInverseMass += ParticleB->GetInverseMass();
    }

    if (totalInverseMass <= 0)
    {
        // Both particles has zero inverse mass == infinite mass => no resolution required
        return;
    }

    // Calculate the movement amounts
    Vector3 movePerInverseMass = ContactNormal * (Penetration / totalInverseMass);
    Vector3 movementA = movePerInverseMass * ParticleA->GetInverseMass();
    Vector3 movementB = Vector3::Zero;
    if (ParticleB != NULL)
    {
        movementB = movePerInverseMass * -ParticleB->GetInverseMass();
    }

    // Apply penetration resolution
    Vector3 positionA = ParticleA->GetPosition() + movementA;
    ParticleA->SetPosition(positionA);
    if (ParticleB != NULL)
    {
        Vector3 positionB = ParticleB->GetPosition() + movementB;
        ParticleB->SetPosition(positionB);
    }
}