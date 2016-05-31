#pragma once

#include "Math/Algebra.h"

class PhysicsParticle;

/* A ParticleContact represents two objects in contact.

IMPULSE
An impulse is an instantaneous change in velocity.
Impulse (g) = mass * velocity
The sum of all impulses on an objects equal the change in velocity.

RESTITUTION
The coefficient of restitution controls the speed at which the objects will separate after
colliding, and depends on the physical properties of the objects. 
If the coefficient is 1, the objects will bounce apart with the same speed
If the coefficient is 0, the objects will coalesce and travel together.
*/

// TODO reconcile with CollisionContact in CollisionDetection.h?
class ParticleContact
{
public:
    PhysicsParticle* ParticleA;
    PhysicsParticle* ParticleB;                         // May be NULL, for contact between object and immovable geometry (e.g. ground)

    float   Restitution;                                // The normal restitution coefficient at the contact
    float   Penetration;                                // The depth of penetration at the contact
    Vector3 ContactNormal;                              // The direction of the contact, from ParticleA's perspective, in world space

protected:
    void    Resolve(float duration);                    // Resolves this contact for both velocity and interpenetration
    float   CalculateSeparatingVelocity();              // Calculate the separating velocity at this contact

private:
    void    ResolveVelocity(float duration);            // Handles the impulse calculations for this collision
    void    ResolveInterpenetration(float duration);    // Handles the interpenetration resolution for this contact
};