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
    Vector3         m_gravity;

};

class ParticleDrag : public ParticleForceGenerator
{
public:
    ParticleDrag(float k1, float k2);
    virtual void    UpdateForce(PhysicsParticle* particle, float deltaTime);

private:
    float           m_k1;
    float           m_k2;
};

class ParticleSpring : public ParticleForceGenerator
{
public:
    ParticleSpring(PhysicsParticle* other, float springConstant, float restLength);
    virtual void UpdateForce(PhysicsParticle* particle, float deltaTime);

private:
    PhysicsParticle* m_other;
    float            m_springConstant;
    float            m_restLength;
};

class ParticleAnchoredSpring : public ParticleForceGenerator
{
public:
    ParticleAnchoredSpring(Vector3& anchor, float springConstant, float restLength);
    virtual void UpdateForce(PhysicsParticle* particle, float deltaTime);

private:
    Vector3         m_anchor;
    float           m_sprintConstant;
    float           m_restLength;
};

class ParticleBungee : public ParticleForceGenerator
{
public:
    ParticleBungee(PhysicsParticle* other, float springConstant, float restLength);
    virtual void UpdateForce(PhysicsParticle* particle, float deltaTime);

private:
    PhysicsParticle* m_other;
    float            m_springConstant;
    float            m_restLength;
};

class ParticleBuoyancy : public ParticleForceGenerator
{
public:
    ParticleBuoyancy(float maxDepth, float volume, float waterHeight, float liquidDensity = 1000.0f);
    virtual void UpdateForce(PhysicsParticle* particle, float deltaTime);

private:
    float           m_maxDepth;
    float           m_volume;
    float           m_waterHeight;
    float           m_liquidDensity;
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