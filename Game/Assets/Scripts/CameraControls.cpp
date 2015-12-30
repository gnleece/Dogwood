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

    float rstickX = InputManager::Singleton().GetGamePad(0)->GetAxisValue(GAMEPAD_RSTICK_X);
    float rstickY = InputManager::Singleton().GetGamePad(0)->GetAxisValue(GAMEPAD_RSTICK_Y);

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
}

void CameraControls::ApplyCameraSettings(Transform& cameraTransform)
{
    Vector3 cameraPosition = cameraTransform.GetWorldPosition();
    Vector3 cameraDirection = cameraTransform.GetForward();
    Camera cam(cameraPosition, cameraDirection, Vector3::Up);
    RenderManager::Singleton().SetCamera(cam);
}