#pragma once

#include "GameComponent.h"

class Bouncer : public GameComponent
{
public:
#pragma region Serializable
    float BounceHeight;
    float BounceSpeed = 1.337f;
#pragma endregion

    void OnCreate();
    void OnStart();
};