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
    float lstickX = 0;
    float lstickY = 0;
    if (InputManager::Singleton().GetGamePad(0)->Connected())
    {
        // Use the direction of the left joystick as a "push" vector to roll the object.
        // The x direction of the joystick applies a push in the x direction in camera space,
        // and the y direction of the joystick applies a push in the z direction in camera space.

        lstickX = InputManager::Singleton().GetGamePad(0)->GetAxisValue(GAMEPAD_LSTICK_X);
        lstickY = InputManager::Singleton().GetGamePad(0)->GetAxisValue(GAMEPAD_LSTICK_Y);
    }
    else
    {
        // No gamepad is connected, so use keyboard controls
        // Use "A" and "D as left stick horizontal and "W" and "S" as left stick vertical

        if (InputManager::Singleton().GetKeyPressed(DGWD_KEY_A))
        {
            lstickX -= 1.0f;
        }
        if (InputManager::Singleton().GetKeyPressed(DGWD_KEY_D))
        {
            lstickX += 1.0f;
        }
        if (InputManager::Singleton().GetKeyPressed(DGWD_KEY_W))
        {
            lstickY += 1.0f;
        }
        if (InputManager::Singleton().GetKeyPressed(DGWD_KEY_S))
        {
            lstickY -= 1.0f;
        }
    }


    // Calculate the camera space direction and convert it to object space
    float pushDelta = Speed * deltaTime;
    Vector3 directionCameraSpace = Vector3(lstickX * pushDelta, 0.0f, -lstickY * pushDelta);
    Vector3 directionWorldSpace = CameraObject.GetGameObject()->GetTransform().TransformVector(directionCameraSpace);
    directionWorldSpace.SetY(0.0f);
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

void Roller::OnCollisionEnter()
{
    printf("\t\tRoller: collision enter\n");
}

void Roller::OnCollisionExit()
{
    printf("\t\tRoller: collision exit\n");
}

void Roller::OnCollisionHold()
{
    printf("\t\tRoller: collision hold\n");
}