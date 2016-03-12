#pragma once

#include "GameComponent.h"
#include "Math/Algebra.h"
#include "Rendering/Color.h"

class EffectParticle;

class ParticleSystem : public GameComponent
{
public:
#pragma region Serializable
    ColorRGB    Color;
    float       MinAge;
    float       MaxAge;
    Vector3     MinVelocity;
    Vector3     MaxVelocity;
#pragma endregion

    void OnCreate();
    void OnStart();
    void Update(float deltaTime);

    void Create(EffectParticle* particle, EffectParticle* parent);
};