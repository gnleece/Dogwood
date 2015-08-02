#include "Roller.h"

#include "Math/Transformations.h"
#include "Input/InputManager.h"
#include "Input/GamePad.h"
#include "Rendering/RenderManager.h"

void Roller::Update(float deltaTime)
{
    if (!m_setupDone)
    {
        m_parentMatrix = m_gameObject->GetLocalTransform().GetMatrix();
        m_setupDone = true;
    }
    
    float lstick = InputManager::Singleton().GetGamePad(0)->GetAxisValue(GAMEPAD_LSTICK_Y);
    float rstick = InputManager::Singleton().GetGamePad(0)->GetAxisValue(GAMEPAD_RSTICK_X);

    float rot_amt = lstick * Speed * deltaTime;
    float trans_amt = rot_amt * Radius/5;
    float turn_amt = -rstick * Speed * deltaTime;

    Matrix4x4 rChild = RotationEulerAngles(Vector3(rot_amt, 0, 0));
    Matrix4x4 rParent = RotationEulerAngles(Vector3(0, turn_amt, 0));
    Matrix4x4 t = Translation(Vector3(0, 0, trans_amt));
    
    m_parentMatrix = m_parentMatrix * t * rParent;
    m_childMatrix = m_childMatrix * rChild;

    m_gameObject->SetLocalTransform(m_parentMatrix*m_childMatrix);

    Vector3 forward = (m_parentMatrix*Vector4(Vector3::Forward, 0)).xyz().Normalized();
    Vector3 objectPosition = m_gameObject->GetLocalTransform().GetPosition();
    Vector3 cameraPosition = objectPosition - 2.5 * forward;
    cameraPosition[1] = cameraPosition[1] + 0.7f;
    Vector3 cameraDirection = forward;

    Camera cam(cameraPosition, cameraDirection, Vector3::Up);
    RenderManager::Singleton().SetCamera(cam);

    GameObject* child = RotationChild.GetGameObject();
    if (child != NULL)
    {
        int x = 0;
    }
}