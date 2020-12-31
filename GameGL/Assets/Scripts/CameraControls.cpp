#include "CameraControls.h"

#include "GameObject.h"
#include "Input/InputManager.h"
#include "Input/GamePad.h"
#include "Rendering/RenderManager.h"

void CameraControls::OnCreate()
{
    printf("\t\tCameraControls OnCreate\n");
}

void CameraControls::OnStart()
{
    printf("\t\tCameraControls OnStart\n");

    // Set initial camera position
    GameObject* targetGO = TargetObject.GetGameObject();
    GameObject* cameraGO = CameraObject.GetGameObject();
    if (targetGO == NULL || cameraGO == NULL)
        return;

    Transform targetTransform = targetGO->GetTransform();
    Vector3 targetPosition = targetTransform.GetWorldPosition();
    Vector3 targetForward = targetTransform.GetForward();

    // Position the camera parent at the target object's position
    m_gameObject->GetTransform().SetLocalPosition(targetPosition);
    m_gameObject->GetTransform().SetLocalRotation(targetTransform.GetLocalRotation());

    // Position the camera object behind the target position
    cameraGO->GetTransform().SetLocalPosition(Vector3(0.0f, Height, -Distance));

    ApplyCameraSettings(cameraGO->GetTransform());
}

void CameraControls::Update(float deltaTime)
{
    GameObject* targetGO = TargetObject.GetGameObject();
    GameObject* cameraGO = CameraObject.GetGameObject();
    if (targetGO == NULL || cameraGO == NULL)
        return;

    float rstickX, rstickY;
    GetInputAxisValues(rstickX, rstickY);

    Transform targetTransform = targetGO->GetTransform();
    Transform cameraTransform = cameraGO->GetTransform();
    
    // Position the camera parent at the target object's position
    Vector3 targetPosition = targetTransform.GetWorldPosition();
    m_gameObject->GetTransform().SetLocalPosition(targetPosition);

    // Rotate the camera parent based on joystick input
    float horizAngleDelta = -rstickX * HorizSpeed * deltaTime;
    float vertAngleDelta = -rstickY * VertSpeed * deltaTime;
    m_horizRotation += horizAngleDelta;
    m_vertRotation = Clamp(m_vertRotation + vertAngleDelta, VertMin, VertMax);
    m_gameObject->GetTransform().SetLocalRotation(Vector3(m_vertRotation, m_horizRotation, 0.0f));

    ApplyCameraSettings(cameraTransform);
    //RenderManager::Singleton().SetCameraTransform(cameraTransform.GetWorldMatrix());
}

void CameraControls::ApplyCameraSettings(Transform& cameraTransform)
{
    Vector3 cameraPosition = cameraTransform.GetWorldPosition();
    Vector3 cameraDirection = cameraTransform.GetForward();
    RenderManager::Singleton()->GetCamera().SetCameraPositionDirectionUp(cameraPosition, cameraDirection, Vector3::Up);

    Transform renderCameraTransform = RenderManager::Singleton()->GetCamera().GetCameraTransform();

    Vector3 newCameraPosition = renderCameraTransform.GetWorldPosition();
    Vector3 newCameraDirection = renderCameraTransform.GetForward();

    Vector3 x;
}

void CameraControls::GetInputAxisValues(float& x, float& y)
{
    x = 0.0f;
    y = 0.0f;
    if (InputManager::Singleton().GetGamePad(0)->Connected())
    {
        x = InputManager::Singleton().GetGamePad(0)->GetAxisValue(GAMEPAD_RSTICK_X);
        y = InputManager::Singleton().GetGamePad(0)->GetAxisValue(GAMEPAD_RSTICK_Y);
    }
    else
    {
        // No gamepad is connected, so use keyboard controls
        if (InputManager::Singleton().GetKeyPressed(DGWD_KEY_LEFT))
        {
            x -= 1.0f;
        }
        if (InputManager::Singleton().GetKeyPressed(DGWD_KEY_RIGHT))
        {
            x += 1.0f;
        }
        if (InputManager::Singleton().GetKeyPressed(DGWD_KEY_UP))
        {
            y += 1.0f;
        }
        if (InputManager::Singleton().GetKeyPressed(DGWD_KEY_DOWN))
        {
            y-= 1.0f;
        }
    }
}