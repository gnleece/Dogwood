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

    RigidBody*          Body[2];                                    // Body[1] may be NULL, for contact between an object and immovable geometry (e.g. ground)

    Vector3             ContactPoint;                               // The position of the contact in world space
    Vector3             ContactNormal;                              // The direction of the contact, from BodyA's perspective, in world space

    float               Restitution;                                // The normal restitution coefficient at the contact
    float               Penetration;                                // The depth of penetration at the contact
    float               Friction;                                   // The lateral friction coefficient at the contact

protected:
    const static float  MIN_VELOCITY_LIMIT;

    void                SwapBodies();

    void                CalculateInternals(float deltaTime);        // Calculate internal values from state values, called before resolution starts
    void                CalculateContactBasis();                    // Calculate an orthonormal basis for the contact point
    Vector3             CalculateLocalVelocity(RigidBody* body, float deltaTime, Vector3 relativeContactPosition);
    void                CalculateDesiredDeltaVelocity(float deltaTime);
    float               CalculateSeparatingVelocity();              // Calculate the separating velocity at this contact
    Vector3             CalculateFrictionlessImpulse(Matrix3x3* inverseInertiaTensor);
    Vector3             CalculateFrictionImpulse(Matrix3x3* inverseInertiaTensor);

    void                ApplyPositionChange(Vector3* linearChange, Vector3* angularChange, float penetration);
    void                ApplyVelocityChange(Vector3* velocityChange, Vector3* angularVelocityChange);

    void                MatchAwakeState();                          // A sleeping body will be made awake if it is in contact with another body that is awake

    Matrix3x3           m_contactToWorld;                           // Transform matrix to convert from contact coords to world space
    Vector3             m_contactVelocity;                          // Closing velocity at point of contact. Set by calculateInternals
    float               m_desiredDeltaVelocity;                     // Holds required change in velocity required to resolve this contact
    Vector3             m_relativeContactPosition[2];               // Worldspace position of contact point relative to center of each body. Set by calculateInternals
};

class ContactResolver
{
public:
    ContactResolver(unsigned int maxIterations);

    void                SetMaxIterations(unsigned int maxIterations);
    void                ResolveContacts(RigidBodyContact* contacts, unsigned int numContacts, float deltaTime);

protected:
    void                PrepareContacts(RigidBodyContact* contacts, unsigned int numContacts, float deltaTime);
    void                AdjustPositions(RigidBodyContact* contacts, unsigned int numContacts, float deltaTime);
    void                AdjustVelocities(RigidBodyContact* contacts, unsigned int numContacts, float deltaTime);

    unsigned int        m_maxIterations;
};