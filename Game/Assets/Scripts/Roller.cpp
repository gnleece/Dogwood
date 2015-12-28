#include "Roller.h"

#include "GameObject.h"
#include "Math/Transformations.h"
#include "Input/InputManager.h"
#include "Input/GamePad.h"

void Roller::OnCreate()
{
    printf("\t\tRoller OnCreate\n");
}

void Roller::OnStart()
{
    printf("\t\tRoller OnStart\n");
}

void Roller::Update(float deltaTime)
{
    // Use the direction of the left joystick as a "push" vector to roll the object.
    // The x direction of the joystick applies a push in the x direction in camera space,
    // and the y direction of the joystick applies a push in the z direction in camera space.

    float lstickX = InputManager::Singleton().GetGamePad(0)->GetAxisValue(GAMEPAD_LSTICK_X);
    float lstickY = InputManager::Singleton().GetGamePad(0)->GetAxisValue(GAMEPAD_LSTICK_Y);

    // Calculate the camera space direction and convert it to object space
    float pushDelta = Speed * deltaTime;
    Vector3 directionCameraSpace = Vector3(-lstickX * pushDelta, 0.0f, lstickY * pushDelta);
    Vector3 directionWorldSpace = CameraObject.GetGameObject()->GetTransform().TransformVector(directionCameraSpace);
    Vector3 directionObjectSpace = m_gameObject->GetTransform().InverseTransformVector(directionWorldSpace);
    Vector3 upObjectSpace = m_gameObject->GetTransform().InverseTransformVector(Vector3::Up);

    if (directionObjectSpace.Magnitude() > 0.0000001f)
    {
        Vector3 rotationAxis = directionObjectSpace.Cross(upObjectSpace).Normalized();
        float rotationAngle = -1 * directionObjectSpace.Magnitude();

        // TODO set world translation instead of local translation to use proper formula with radius
        float translationAmount = DegreesToRadians(-1 * rotationAngle);

        Matrix4x4 r = Rotation(rotationAngle, rotationAxis);
        Vector3 trans = directionObjectSpace.Normalized() * translationAmount;
        Matrix4x4 t = Translation(trans);

        Matrix4x4 m = m_gameObject->GetTransform().GetWorldMatrix() * t * r;
        m_gameObject->GetTransform().SetLocalMatrix(m);
    }
}