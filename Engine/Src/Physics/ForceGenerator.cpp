#include "Physics/ForceGenerator.h"
#include "Physics/Particles/PhysicsParticle.h"
#include <algorithm>

void GravityGenerator::UpdateForce(PhysicsParticle* particle, float deltaTime)
{
    if (!particle->HasFiniteMass())
        return;

    // Apply the mass-scaled force to the particle
    particle->AddForce(m_gravity * particle->GetMass());
}

//-----------------------------------------------------------------------------------------------

void DragGenerator::UpdateForce(PhysicsParticle* particle, float deltaTime)
{
    Vector3 force = particle->GetVelocity();

    // Calculate the total drag coefficient
    float dragCoeff = force.Magnitude();
    dragCoeff = (m_k1 * dragCoeff) + (m_k2 * dragCoeff * dragCoeff);

    // Calculate the final force and apply it
    force.Normalize();
    force = force * -dragCoeff;
    particle->AddForce(force);
}

//-----------------------------------------------------------------------------------------------

void SpringGenerator::UpdateForce(PhysicsParticle* particle, float deltaTime)
{
    // Calculate the vector of the spring
    Vector3 force = particle->GetPosition() - m_other->GetPosition();

    // Calculate the magnitude of the force
    float magnitude = force.Magnitude();
    magnitude = abs(magnitude - m_restLength);
    magnitude *= m_springConstant;

    // Calculate the final force and apply it
    force.Normalize();
    force = force * -magnitude;
    particle->AddForce(force);
}

//-----------------------------------------------------------------------------------------------

void AnchoredSpringGenerator::UpdateForce(PhysicsParticle* particle, float deltaTime)
{
    // Calculate the vector of the spring
    Vector3 force = particle->GetPosition() - m_anchor;

    // Calculate the magnitude of the force
    float magnitude = force.Magnitude();
    magnitude = (m_restLength - magnitude) * m_sprintConstant;

    // Calculate the final force and apply it
    force.Normalize();
    force = force * magnitude;
    particle->AddForce(force);
}

//-----------------------------------------------------------------------------------------------

void BungeeGenerator::UpdateForce(PhysicsParticle* particle, float deltaTime)
{
    // Calculate the vector of the spring
    Vector3 force = particle->GetPosition() - m_other->GetPosition();

    // Check if the bungee is compressed
    float magnitude = force.Magnitude();
    if (magnitude <= m_restLength)
        return;

    // Calculate the magnitude of the force
    magnitude = (m_restLength * magnitude) * m_springConstant;

    // Calculate the final force and apply it
    force.Normalize();
    force = force * -magnitude;
    particle->AddForce(force);
}

//-----------------------------------------------------------------------------------------------

void BuoyancyGenerator::UpdateForce(PhysicsParticle* particle, float deltaTime)
{
    // Calculate the submersion depth
    float depth = particle->GetPosition().y();

    // Check if we're out of the water
    if (depth >= m_waterHeight + m_maxDepth)
        return;

    // Check if we're at maximum depth
    if (depth <= m_waterHeight - m_maxDepth)
    {
        Vector3 force;
        force.SetY(m_liquidDensity * m_volume);
        particle->AddForce(force);
    }
    // Otherwise we are partly submerged
    else
    {
        Vector3 force;
        force.SetY(m_liquidDensity * m_volume * (depth - m_maxDepth - m_waterHeight) / 2 * m_maxDepth);
        particle->AddForce(force);
    }
}

//-----------------------------------------------------------------------------------------------

void ForceRegistry::Register(PhysicsParticle* particle, ForceGenerator* generator)
{
    RegistrationInfo info(particle, generator);
    m_registry.push_back(info);
}

void ForceRegistry::Unregister(PhysicsParticle* particle, ForceGenerator* generator)
{
    RegistrationInfo info(particle, generator);
    m_registry.erase(
        std::remove(m_registry.begin(), m_registry.end(), info),
        m_registry.end());
}

void ForceRegistry::Clear()
{
    m_registry.clear();
}

void ForceRegistry::UpdateForces(float deltaTime)
{
    vector<RegistrationInfo>::iterator iter;
    for (iter = m_registry.begin(); iter != m_registry.end(); iter++)
    {
        iter->Generator->UpdateForce(iter->Particle, deltaTime);
    }
}

ForceRegistry::RegistrationInfo::RegistrationInfo(PhysicsParticle* particle, ForceGenerator* generator)
{
    Particle = particle;
    Generator = generator;
}

bool ForceRegistry::RegistrationInfo::operator==(const RegistrationInfo &other) const
{
    return Particle == other.Particle && Generator == other.Generator;
}