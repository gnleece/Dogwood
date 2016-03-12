#include "Components/ParticleSystem.h"

#include "Math/MathUtil.h"
#include "Physics/Particles/EffectParticle.h"

void ParticleSystem::OnCreate()
{
    printf("\t\tParticleSystem OnCreate\n");
}

void ParticleSystem::OnStart()
{
    printf("\t\tParticleSystem OnStart\n");
}

void ParticleSystem::Update(float deltaTime)
{
}

void ParticleSystem::Create(EffectParticle* particle, EffectParticle* parent)
{
    //particle->Type = Type;
    particle->Age = RandomFloat(MinAge, MaxAge);

    Vector3 velocity;
    if (parent != NULL)
    {
        // Position and velocity are based on the parent
        particle->SetPosition(parent->GetPosition());
        velocity += parent->GetVelocity();
    }
    else
    {
        // Start position is random
        Vector3 startPosition;
        int x = RandomInt(0, 2);
        startPosition.SetX(5.0f * x);
        particle->SetPosition(startPosition);
    }

    velocity += RandomVector(Vector3::Zero, 3 * Vector3::One);
    particle->SetVelocity(velocity);
    particle->SetMass(1.0f);
    particle->SetAcceleration(Vector3(0, -10, 0));      // TODO gravity constant
    particle->ClearAccumulator();
}