#pragma once

#include "Debugging\DebugDraw.h"
#include "Math\Algebra.h"
#include "Math\Transform.h"
#include "Tools\BaseSceneTool.h"

class TransformTool : public BaseSceneTool
{
public:
    enum eMode { TOOL_MODE_TRANSLATE, TOOL_MODE_ROTATE, TOOL_MODE_SCALE };

    virtual void    Init(SceneViewWidget* parent);
    virtual void    Draw(Transform& transform);
    void            SetLocalTransform(Transform& transform);

    void            SetMode(eMode mode);
    eMode           GetMode();

    virtual bool    OnMouseDown(int screenX, int screenY, Vector3 rayOrigin, Vector3 rayDirection);
    virtual void    OnMouseMove(int screenX, int screenY);
    virtual void    OnMouseUp();

private:
    float       CalculateT(float screenX, float screenY);
    void        ApplyTranslation(float direction, float scale);
    void        ApplyRotation();
    void        ApplyScale(float direction, float scale);

    eMode        m_mode;

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