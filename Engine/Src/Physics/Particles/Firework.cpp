#include "Physics/Particles/Firework.h"

#include "Util.h"

bool Firework::Update(float deltaTime)
{
    Integrate(deltaTime);

    Age -= deltaTime;

    return (Age < 0) || (m_position.y() < 0);
}

void FireworkRule::Create(Firework* firework, const Firework* parent)
{
    firework->Type = Type;
    firework->Age = RandomFloat(MinAge, MaxAge);

    Vector3 velocity;
    if (parent != NULL)
    {

    }
}