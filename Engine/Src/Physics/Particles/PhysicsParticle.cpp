#include "Physics/Particles/PhysicsParticle.h"
#include <assert.h>
#include <math.h>

void PhysicsParticle::SetPosition(Vector3 position)
{
    m_position = position;
}

Vector3 PhysicsParticle::GetPosition()
{
    return m_position;
}

void PhysicsParticle::SetVelocity(Vector3 velocity)
{
    m_velocity = velocity;
}

Vector3 PhysicsParticle::GetVelocity()
{
    return m_velocity;
}

void PhysicsParticle::SetAcceleration(Vector3 acceleration)
{
    m_acceleration = acceleration;
}

Vector3 PhysicsParticle::GetAcceleration()
{
    return m_acceleration;
}

void PhysicsParticle::SetMass(float mass)
{
    if (mass < 0 || Approximately(mass, 0.f))
    {
        return;
    }
    m_inverseMass = 1 / mass;
}

void PhysicsParticle::SetInverseMass(float inverseMass)
{
    if (inverseMass < 0)
        return;
    m_inverseMass = inverseMass;
}

float PhysicsParticle::GetInverseMass()
{
    return m_inverseMass;
}


void PhysicsParticle::Integrate(float deltaTime)
{
    if (m_inverseMass <= 0.f)
        return;

    assert(deltaTime > 0.0);

    // Update position (deltaTime is small so acceleration term is negligible here)
    m_position = m_position + deltaTime * m_velocity;

    // Calculate acceleration from force
    Vector3 netAcceleration = m_acceleration;

    // Update velocity
    m_velocity = m_velocity + deltaTime * netAcceleration;

    // Apply damping
    m_velocity = pow(DAMPING, deltaTime) * m_velocity;      // TODO calculate pow(DAMPING, dt) once per frame
}

void PhysicsParticle::ClearAccumulator()
{
    // TODO implement me
}