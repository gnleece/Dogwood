#include "DebugCameraControls.h"
#include "..\Input\InputManager.h"
#include "..\Rendering\RenderManager.h"

void DebugCameraControls::Update()
{
    CheckKeys();
    CheckMouse();
}

void DebugCameraControls::CheckKeys()
{
    if (InputManager::Singleton().GetKeyPressed(GLFW_KEY_UP))
    {
        RotateCamera(AXIS_X, -KEY_ROT_AMOUNT);
    }
    if (InputManager::Singleton().GetKeyPressed(GLFW_KEY_DOWN))
    {
        RotateCamera(AXIS_X, KEY_ROT_AMOUNT);
    }
    if (InputManager::Singleton().GetKeyPressed(GLFW_KEY_LEFT))
    {
        RotateCamera(AXIS_Y, -KEY_ROT_AMOUNT);
    }
    if (InputManager::Singleton().GetKeyPressed(GLFW_KEY_RIGHT))
    {
        RotateCamera(AXIS_Y, KEY_ROT_AMOUNT);
    }
    if (InputManager::Singleton().GetKeyPressed(GLFW_KEY_W))
    {
        MoveCamera(Vector3(0, 0, KEY_TRANS_AMOUNT));
    }
    if (InputManager::Singleton().GetKeyPressed(GLFW_KEY_S))
    {
        MoveCamera(Vector3(0, 0, -KEY_TRANS_AMOUNT));
    }
    if (InputManager::Singleton().GetKeyPressed(GLFW_KEY_A))
    {
        MoveCamera(Vector3(KEY_TRANS_AMOUNT, 0, 0));
    }
    if (InputManager::Singleton().GetKeyPressed(GLFW_KEY_D))
    {
        MoveCamera(Vector3(-KEY_TRANS_AMOUNT, 0, 0));
    }
}

void DebugCameraControls::CheckMouse()
{
    if (InputManager::Singleton().GetMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
    {
        CursorPos pos = InputManager::Singleton().GetCursorPos();
        if (m_mouseDragging)
        {
            float deltaX = pos.first - m_prevXpos;
            float deltaY = pos.second - m_prevYpos;
            RotateCamera(AXIS_Y, deltaX*MOUSE_ROT_AMOUNT);
            RotateCamera(AXIS_X, deltaY*MOUSE_ROT_AMOUNT);
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

void DebugCameraControls::MoveCamera(Vector3 localSpaceOffset)
{
    Matrix4x4 view = RenderManager::Singleton().GetView();
    view = Translation(localSpaceOffset)*view;
    RenderManager::Singleton().SetView(view);
}

void DebugCameraControls::RotateCamera(eAXIS axis, float degrees)
{
    Matrix4x4 view = RenderManager::Singleton().GetView();
    view = Rotation(degrees, axis)*view;
    RenderManager::Singleton().SetView(view);
}