#pragma once

#include "Math/Algebra.h"

class Particle
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
    float       GetInverseMass();

    // Integrates the particle forward in time (i.e. updates position and velocity)
    void        Integrate(float deltaTime);
    void        ClearAccumulator();

protected:
    Vector3     m_position;
    Vector3     m_velocity;
    Vector3     m_acceleration;

    // Inverse mass is used here instead of simply mass, so that we can represent "infinite" mass (with an
    // inverse mass of 0), and *cannot* represent zero mass (which we don't want to allow).
    float       m_inverseMass;

    // Damping is required to remove energy added from numerical instability in physics integration step.
    const float DAMPING = 0.999f;
};