#include "Physics/Particles/Particle.h"
#include <assert.h>
#include <math.h>

void Particle::SetMass(float mass)
{
    if (mass < 0 || Approximately(mass, 0.f))
    {
        return;
    }
    m_inverseMass = 1 / mass;
}

void Particle::SetInverseMass(float inverseMass)
{
    if (inverseMass < 0)
        return;
    m_inverseMass = inverseMass;
}

void Particle::Integrate(float deltaTime)
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