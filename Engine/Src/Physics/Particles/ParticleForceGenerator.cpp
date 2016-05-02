#include "Physics/Particles/ParticleForceGenerator.h"
#include "Physics/Particles/PhysicsParticle.h"
#include <algorithm>

void ParticleGravity::UpdateForce(PhysicsParticle* particle, float deltaTime)
{
    if (!particle->HasFiniteMass())
        return;

    // Apply the mass-scaled force to the particle
    particle->AddForce(m_gravity * particle->GetMass());
}

//-----------------------------------------------------------------------------------------------

void ParticleForceRegistry::Register(PhysicsParticle* particle, ParticleForceGenerator* generator)
{
    RegistrationInfo info(particle, generator);
    m_registry.push_back(info);
}

void ParticleForceRegistry::Unregister(PhysicsParticle* particle, ParticleForceGenerator* generator)
{
    RegistrationInfo info(particle, generator);
    m_registry.erase(
        std::remove(m_registry.begin(), m_registry.end(), info),
        m_registry.end());
}

void ParticleForceRegistry::Clear()
{
    m_registry.clear();
}

void ParticleForceRegistry::UpdateForces(float deltaTime)
{
    vector<RegistrationInfo>::iterator iter;
    for (iter = m_registry.begin(); iter != m_registry.end(); iter++)
    {
        iter->Generator->UpdateForce(iter->Particle, deltaTime);
    }
}

ParticleForceRegistry::RegistrationInfo::RegistrationInfo(PhysicsParticle* particle, ParticleForceGenerator* generator)
{
    Particle = particle;
    Generator = generator;
}

bool ParticleForceRegistry::RegistrationInfo::operator==(const RegistrationInfo &other) const
{
    return Particle == other.Particle && Generator == other.Generator;
}