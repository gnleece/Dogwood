#pragma once

#include "..\Math\Algebra.h"

class DebugCameraControls
{
public:
    const float KEY_TRANS_AMOUNT = 0.002f;
    const float KEY_ROT_AMOUNT = 0.05f;
    const float MOUSE_ROT_AMOUNT = 0.07f;

    void Update();

private:
    void CheckKeys();
    void CheckMouse();
    void MoveCamera(Vector3 localSpaceOffset);
    void RotateCamera(eAXIS axis, float degrees);

    float   m_prevXpos;
    float   m_prevYpos;
    bool    m_mouseDragging = false;
};