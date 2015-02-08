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
    if (InputManager::Singleton().GetKeyPressed(GLFW_KEY_UP))
    {
        RotateCamera(AXIS_X, -KEY_ROT_AMOUNT*deltaTime);
    }
    if (InputManager::Singleton().GetKeyPressed(GLFW_KEY_DOWN))
    {
        RotateCamera(AXIS_X, KEY_ROT_AMOUNT*deltaTime);
    }
    if (InputManager::Singleton().GetKeyPressed(GLFW_KEY_LEFT))
    {
        RotateCamera(AXIS_Y, -KEY_ROT_AMOUNT*deltaTime);
    }
    if (InputManager::Singleton().GetKeyPressed(GLFW_KEY_RIGHT))
    {
        RotateCamera(AXIS_Y, KEY_ROT_AMOUNT*deltaTime);
    }
    if (InputManager::Singleton().GetKeyPressed(GLFW_KEY_W))
    {
        MoveCamera(Vector3(0, 0, KEY_TRANS_AMOUNT*deltaTime));
    }
    if (InputManager::Singleton().GetKeyPressed(GLFW_KEY_S))
    {
        MoveCamera(Vector3(0, 0, -KEY_TRANS_AMOUNT*deltaTime));
    }
    if (InputManager::Singleton().GetKeyPressed(GLFW_KEY_A))
    {
        MoveCamera(Vector3(KEY_TRANS_AMOUNT*deltaTime, 0, 0));
    }
    if (InputManager::Singleton().GetKeyPressed(GLFW_KEY_D))
    {
        MoveCamera(Vector3(-KEY_TRANS_AMOUNT*deltaTime, 0, 0));
    }
}

void DebugCameraControls::CheckMouse(float deltaTime)
{
    if (InputManager::Singleton().GetMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
    {
        CursorPos pos = InputManager::Singleton().GetCursorPos();
        if (m_mouseDragging)
        {
            float deltaX = pos.first - m_prevXpos;
            float deltaY = pos.second - m_prevYpos;
            RotateCamera(AXIS_Y, deltaX*MOUSE_ROT_AMOUNT*deltaTime);
            RotateCamera(AXIS_X, deltaY*MOUSE_ROT_AMOUNT*deltaTime);
        }
        m_prevXpos = pos.first;
        m_prevYpos = pos.second;
        m_mouseDragging = true;
    }
    else
    {
        m_mouseDragging = false;
    }
}

void DebugCameraControls::CheckGamePad(float deltaTime)
{
    // TODO temp debugging, remove me
    GamePad* pad = InputManager::Singleton().GetGamePad(0);

    printf("%f\n", pad->GetAxisValue(AbstractGamepadAxes::GAMEPAD_LTRIGGER));
}

void DebugCameraControls::MoveCamera(Vector3 localSpaceOffset)
{
    Matrix4x4 view = RenderManager::Singleton().GetView().GetMatrix();
    view = Translation(localSpaceOffset)*view;
    RenderManager::Singleton().SetView(view);
}

void DebugCameraControls::RotateCamera(eAXIS axis, float degrees)
{
    Matrix4x4 view = RenderManager::Singleton().GetView().GetMatrix();
    view = Rotation(degrees, axis)*view;
    RenderManager::Singleton().SetView(view);
}