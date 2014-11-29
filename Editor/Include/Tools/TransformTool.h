#pragma once

#include "Debugging\DebugDraw.h"
#include "Math\Algebra.h"
#include "Math\Transform.h"

class TransformTool
{
public:
    void        Init();
    void        Draw(Transform& transform);
    bool        OnClick(Vector3 rayOrigin, Vector3 rayDirection);

private:
    Transform   m_transform;
    Gnomon      m_gnomon;

    float       m_arrowBase;
    float       m_arrowHeight;
    Transform   m_arrowTransforms[3];
};