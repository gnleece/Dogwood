#pragma once

#pragma once

#include "Math/Algebra.h"

class RigidBody
{
public:
    void        SetPosition(Vector3 position);
    Vector3     GetPosition();

    void        SetVelocity(Vector3 velocity);
    Vector3     GetVelocity();

    void        SetAcceleration(Vector3 acceleration);
    Vector3     GetAcceleration();

    void        SetMass(float mass);
    void        SetInverseMass(float inverseMass);
    float       GetMass();
    float       GetInverseMass();
    bool        HasFiniteMass();

    // Integrates the particle forward in time (i.e. updates position and velocity)
    void        Integrate(float deltaTime);

    // Adds the given force to the particle to be applied at the next integration only
    void        AddForce(Vector3 force);

    void        ClearAccumulator();

protected:
    Vector3     m_position;
    Quaternion  m_rotation;

    Vector3     m_velocity;
    Vector3     m_angularVelocity;

    Vector3     m_acceleration;

    // Inverse mass is used here instead of simply mass, so that we can represent "infinite" mass (with an
    // inverse mass of 0), and *cannot* represent zero mass (which we don't want to allow).
    float       m_inverseMass;

    // This is calculated from the inverse mass, and then cached. It is not set directly.
    float       m_mass;

    Vector3     m_accumulatedForce;

    // Damping is required to remove energy added from numerical instability in physics integration step.
    const float DAMPING = 0.999f;
};