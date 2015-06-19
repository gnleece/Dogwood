#pragma once

#include "GameComponent.h"


class Translator : public GameComponent
{
public:

#pragma region Serializable
    float   Speed = 0.5f;
#pragma endregion

    void Update(float deltaTime);
};