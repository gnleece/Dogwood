#include "Physics/Particles/Firework.h"

#include "Math/MathUtil.h"

void FireworkRule::Create(Firework* firework, Firework* parent)
{
    firework->Type = Type;
    firework->Age = RandomFloat(MinAge, MaxAge);

    Vector3 velocity;
    if (parent != NULL)
    {
        // Position and velocity are based on the parent
        firework->SetPosition(parent->GetPosition());
        velocity += parent->GetVelocity();
    }
    else
    {
        // Start position is random
        Vector3 startPosition;
        int x = RandomInt(0, 2);
        startPosition.SetX(5.0f * x);
        firework->SetPosition(startPosition);
    }

    velocity += RandomVector(Vector3::Zero, 3 * Vector3::One);
    firework->SetVelocity(velocity);
    firework->SetMass(1.0f);
    firework->SetAcceleration(Vector3(0, -10, 0));      // TODO gravity constant
    firework->ClearAccumulator();
}

bool Firework::Update(float deltaTime)
{
    Integrate(deltaTime);

    Age -= deltaTime;

    return (Age < 0) || (m_position.y() < 0);
}