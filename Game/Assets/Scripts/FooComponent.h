#pragma once

#include "GameComponent.h"

class FooComponent : public GameComponent
{
public:
#pragma region Serializable
    float BarParam;
    bool BazParam;
#pragma endregion
};