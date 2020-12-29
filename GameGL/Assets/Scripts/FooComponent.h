#pragma once

#include "GameComponent.h"

class FooComponent : public GameComponent
{
public:
#pragma region Serializable
    float   BarParam;
    int     IntParam = 12;
#pragma endregion

    void OnCreate();
    void OnStart();
};