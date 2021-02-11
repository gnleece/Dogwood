#include "Debugging\DebugCameraControls.h"
#include "Input\InputManager.h"
#include "Input\GamePad.h"
#include "Rendering\RenderManager.h"

void DebugCameraControls::Update(float deltaTime)
{
    CheckKeys(deltaTime);
    CheckMouse(deltaTime);
    CheckGamePad(deltaTime);
}

void DebugCameraControls::CheckKeys(float deltaTime)
{

}

void DebugCameraControls::CheckMouse(float deltaTime)
{
    
}

void DebugCameraControls::CheckGamePad(float deltaTime)
{
    // TODO temp debugging, remove me
    //GamePad* pad = InputManager::Singleton()->GetGamePad(0);

    //printf("%f\n", pad->GetAxisValue(AbstractGamepadAxes::GAMEPAD_LSTICK_X));
    //printf("%f\n", pad->GetAxisValue(AbstractGamepadAxes::GAMEPAD_LSTICK_Y));
}

void DebugCameraControls::MoveCamera(Vector3 localSpaceOffset)
{
    Transform cameraTransform = RenderManager::Singleton()->GetCamera().GetCameraTransform();
    cameraTransform = Translation(localSpaceOffset)*cameraTransform;
    RenderManager::Singleton()->GetCamera().SetCameraTransform(cameraTransform);
}

void DebugCameraControls::RotateCamera(eAXIS axis, float degrees)
{
    Transform cameraTransform = RenderManager::Singleton()->GetCamera().GetCameraTransform();
    cameraTransform = Rotation(degrees, axis)*cameraTransform;
    RenderManager::Singleton()->GetCamera().SetCameraTransform(cameraTransform);
}