#pragma once

#include "Math/Algebra.h"
#include <vector>

using std::vector;

class PhysicsParticle;

class ParticleForceGenerator
{
public:
    virtual void    UpdateForce(PhysicsParticle* particle, float deltaTime) = 0;
};

class ParticleGravity : public ParticleForceGenerator
{
public:
    ParticleGravity(Vector3& gravity);
    virtual void    UpdateForce(PhysicsParticle* particle, float deltaTime);

private:
    Vector3     m_gravity;

};

class ParticleForceRegistry
{
public:
    void            Register(PhysicsParticle* particle, ParticleForceGenerator* generator);
    void            Unregister(PhysicsParticle* particle, ParticleForceGenerator* generator);
    void            Clear();
    void            UpdateForces(float deltaTime);

protected:
    struct RegistrationInfo
    {
        PhysicsParticle*        Particle;
        ParticleForceGenerator* Generator;

        RegistrationInfo(PhysicsParticle* particle, ParticleForceGenerator* generator);
        bool operator==(const RegistrationInfo &other) const;
    };
    vector<RegistrationInfo> m_registry;
};