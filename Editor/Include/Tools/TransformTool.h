#pragma once

#include "Debugging\DebugDraw.h"
#include "Math\Algebra.h"
#include "Math\Transform.h"
#include "Tools\BaseSceneTool.h"

class TransformTool : public BaseSceneTool
{
public:
    enum eMode { TOOL_MODE_TRANSLATE, TOOL_MODE_ROTATE, TOOL_MODE_SCALE };
    enum eSpace { TOOL_SPACE_LOCAL, TOOL_SPACE_GLOBAL };

    virtual void    Init(SceneViewWidget* parent);
    virtual void    SetTargetTransform(Transform* transform);
    virtual void    Draw();

    void            SetMode(eMode mode);
    eMode           GetMode();

    void            SetSpace(eSpace space);
    eSpace          GetSpace();

    virtual bool    OnMouseDown(int screenX, int screenY, Vector3 rayOrigin, Vector3 rayDirection);
    virtual void    OnMouseMove(int screenX, int screenY);
    virtual void    OnMouseUp();

private:
    float       CalculateT(float screenX, float screenY);
    void        ApplyTranslation(float direction, float scale);
    void        ApplyRotation(float direction, float scale);
    void        ApplyScale(float direction, float scale);
    Transform   UnrotatedTransform(Transform* transform);

    eMode       m_mode;
    eSpace      m_space;

    Transform*  m_transform;
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