#pragma once

#include "Math\Algebra.h"

class DebugCameraControls
{
public:
    const float KEY_TRANS_AMOUNT = 2.f;
    const float KEY_ROT_AMOUNT = 35.f;
    const float MOUSE_ROT_AMOUNT = 7.f;

    void Update(float deltaTime);

private:
    void CheckKeys(float deltaTime);
    void CheckMouse(float deltaTime);
    void CheckGamePad(float deltaTime);

    void MoveCamera(Vector3 localSpaceOffset);
    void RotateCamera(eAXIS axis, float degrees);

    float   m_prevXpos;
    float   m_prevYpos;
    bool    m_mouseDragging = false;
};