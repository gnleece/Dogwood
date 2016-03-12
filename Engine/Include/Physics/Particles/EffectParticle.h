#pragma once

#include "Physics/Particles/PhysicsParticle.h"

class EffectParticle;

struct FireworkRule
{
public:
    struct Payload
    {
    public:
        unsigned int Type;
        unsigned int Count;

        Payload(unsigned int type, unsigned int count)
        {
            Type = type;
            Count = count;
        }
    };

    enum FireworkType {};

    FireworkType Type;
    float MinAge;
    float MaxAge;
    Vector3 MinVelocity;
    Vector3 MaxVelocity;
    float Damping;

    unsigned int PayloadCount;
    Payload* Payload;

    void Create(EffectParticle* firework, EffectParticle* parent = NULL);
};

class EffectParticle : public PhysicsParticle
{
public:
    FireworkRule::FireworkType Type;
    float Age;

    bool Update(float deltaTime);
};