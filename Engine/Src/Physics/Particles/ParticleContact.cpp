#include "Physics/Particles/ParticleContact.h"
#include "Physics/Particles/PhysicsParticle.h"

void ParticleContact::Resolve(float duration)
{
    ResolveVelocity(duration);
    //ResolveInterpenetration(duration);    // TODO implement me
}

// Separating velocity (v_s) = (relative velocity) dot (contact normal) = (v_a - v_b) dot (norm(p_a - p_b))
float ParticleContact::CalculateSeparatingVelocity()
{
    Vector3 relativeVelocity = ParticleA->GetVelocity();
    if (ParticleB != NULL)
    {
        // ParticleB is NULL in the case where ParticleA is colliding with some immovable object (e.g. ground)
        relativeVelocity -= ParticleB->GetVelocity();
    }
    return relativeVelocity.Dot(ContactNormal);
}

void ParticleContact::ResolveVelocity(float duration)
{
    // Calculate the velocity in the direction of the contact
    float separatingVelocity = CalculateSeparatingVelocity();

    // Check whether it needs to be resolved
    if (separatingVelocity > 0)
    {
        // The contact is either separating or stationary, so no impulse is required
        return;
    }

    // Use restitution to calculate the change in velocity
    float newSeparatingVelocity = -Restitution * separatingVelocity;
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
        Vector3 velocityB = ParticleB->GetVelocity() + impulsePerInverseMass * ParticleB->GetInverseMass();
        ParticleB->SetVelocity(velocityB);
    }
}