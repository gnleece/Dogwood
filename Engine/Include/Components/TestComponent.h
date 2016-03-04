#pragma once

#include "GameComponent.h"

class TestComponent : public GameComponent
{
public:
#pragma region Serializable
    int     IntParam;
    bool    BoolParam;
#pragma endregion

    void OnCreate();
    void OnStart();
    void Update(float deltaTime);
};