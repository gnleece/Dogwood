#pragma once

#include "GameComponent.h"
#include "GameObjectReference.h"
#include "Math\Algebra.h"

class Roller : public GameComponent
{
public:

#pragma region Serializable
    float               Speed = 25.0f;
    float               Radius = 0.1f;

    GameObjectReference CameraObject;
#pragma endregion

    void OnCreate();
    void OnStart();
    void Update(float deltaTime);
};