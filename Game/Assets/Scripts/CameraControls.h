#pragma once

#include "GameComponent.h"
#include "GameObjectReference.h"
#include "Math\Algebra.h"
#include "Math\Transform.h"

class CameraControls : public GameComponent
{
public:

#pragma region Serializable
    GameObjectReference TargetObject;
    GameObjectReference CameraObject;

    float               Distance = 3.5f;
    float               Height = 0.65f;

    float               HorizSpeed = 100.0f;
    float               VertSpeed = 100.0f;

    float               VertMin = -20.0f;
    float               VertMax = 35.0f;
#pragma endregion

    void OnCreate();
    void OnStart();
    void Update(float deltaTime);

private:
    void ApplyCameraSettings(Transform& cameraTransform);

    float               m_horizRotation;
    float               m_vertRotation;
};