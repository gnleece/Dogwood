#include "Physics/Particles/EffectParticle.h"

#include "Math/MathUtil.h"

bool EffectParticle::Update(float deltaTime)
{
    Integrate(deltaTime);

    Age -= deltaTime;

    return (Age < 0) || (m_position.y() < 0);
}