#pragma once

#include "..\Engine\GameComponent.h"

class SpinningComponent : public GameComponent
{
public:
    float Speed = 50;

    void Update(float deltaTime);
};