#pragma once

#include "Debugging\DebugDraw.h"
#include "Math\Algebra.h"
#include "Math\Transform.h"
#include "Tools\BaseSceneTool.h"

class TransformTool : public BaseSceneTool
{
public:
    virtual void    Init(SceneViewWidget* parent);
    virtual void    Draw(Transform& transform);

    virtual bool    OnMouseDown(int screenX, int screenY, Vector3 rayOrigin, Vector3 rayDirection);
    virtual void    OnMouseMove(int screenX, int screenY);
    virtual void    OnMouseUp();

private:
    Transform   m_transform;
    Gnomon      m_gnomon;

    float       m_arrowBase;
    float       m_arrowHeight;
    Transform   m_arrowTransforms[3];

    float       m_prevLineX;
    float       m_prevLineY;
    float m_prevT;

    Vector2     m_point0;
    Vector2     m_point1;

    eAXIS       m_activeAxis;
};