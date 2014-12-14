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
    void            SetLocalTransform(Transform& transform);

    virtual bool    OnMouseDown(int screenX, int screenY, Vector3 rayOrigin, Vector3 rayDirection);
    virtual void    OnMouseMove(int screenX, int screenY);
    virtual void    OnMouseUp();

private:
    float       CalculateT(float screenX, float screenY);

    Transform   m_transform;
    Transform   m_localTransform;
    Gnomon      m_gnomon;

    float       m_arrowBase;
    float       m_arrowHeight;
    Transform   m_arrowTransforms[3];

    eAXIS       m_activeAxis;

    Vector2     m_activeAxisPoint0;
    Vector2     m_activeAxisPoint1;
    bool        m_vertical;
    float       m_a;
    float       m_c;
    float       m_prevT;
};