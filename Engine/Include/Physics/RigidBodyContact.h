#pragma once

#include "Math/Algebra.h"

class RigidBody;

// A RigidBodyContact represents two (rigid body) objects in contact.
//
// IMPULSE
// An impulse is an instantaneous change in velocity.
// Impulse (g) = mass * velocity
// The sum of all impulses on an objects equal the change in velocity.
//
// RESTITUTION
// The coefficient of restitution controls the speed at which the objects will separate after
// colliding, and depends on the physical properties of the objects.
// If the coefficient is 1, the objects will bounce apart with the same speed
// If the coefficient is 0, the objects will coalesce and travel together.

class RigidBodyContact
{
friend class ContactResolver;

public:

    struct ResolutionResult
    {
        Vector3 MovementA;
        Vector3 MovementB;
    };

    RigidBody*          BodyA;
    RigidBody*          BodyB;                                      // May be NULL, for contact between object and immovable geometry (e.g. ground)

    Vector3             ContactPoint;                               // The position of the contact in world space
    Vector3             ContactNormal;                              // The direction of the contact, from BodyA's perspective, in world space

    float               Restitution;                                // The normal restitution coefficient at the contact
    float               Penetration;                                // The depth of penetration at the contact

protected:
    void                CalculateInternals(float deltaTime);        // Calculate internal values from state values, called before resolution starts
    void                CalculateContactBasis();                    // Calculate an orthonormal basis for the contact point
    Vector3             CalculateLocalVelocity(RigidBody* body, float deltaTime);
    void                CalculateDesiredDeltaVelocity(float deltaTime);

    ResolutionResult    Resolve(float deltaTime);                   // Resolves this contact for both velocity and interpenetration
    float               CalculateSeparatingVelocity();              // Calculate the separating velocity at this contact

    void                CalculateFrictionlessImpulse(Matrix3x3* inverseInertiaTensor);

    void                ResolveVelocity(float deltaTime);           // Handles the impulse calculations for this collision
    ResolutionResult    ResolveInterpenetration(float deltaTime);   // Handles the interpenetration resolution for this contact

    void                ApplyPositionChange(Vector3* linearChange, Vector3* angularChange, float penetration);
    void                SwapBodies();

    Matrix3x3           m_contactToWorld;                           // Transform matrix to convert from contact coords to world space
    Vector3             m_contactVelocity;                          // Closing velocity at point of contact. Set by calculateInternals
    float               m_desiredDeltaVelocity;                     // Holds required change in velocity required to resolve this contact.
    Vector3             m_relativeContactPosition[2];               // Worldspace position of contact point relative to center of each body. Set by calculateInternals
};

class ContactResolver
{
public:
    ContactResolver(unsigned int iterations);

    void                SetIterations(unsigned int iterations);
    void                ResolveContacts(RigidBodyContact* contacts, unsigned int numContacts, float deltaTime);

protected:
    void                PrepareContacts(RigidBodyContact* contacts, unsigned int numContacts, float deltaTime);
    void                AdjustPositions(RigidBodyContact* contacts, unsigned int numContacts, float deltaTime);
    void                AdjustVelocities(RigidBodyContact* contacts, unsigned int numContacts, float deltaTime);

    unsigned int        m_iterations;
};