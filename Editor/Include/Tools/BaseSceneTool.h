#pragma once

#include "Math\Algebra.h"
#include "Math\Transform.h"

class SceneViewWidget;

class BaseSceneTool
{
public:
    virtual void Init(SceneViewWidget* parent);
    virtual void Draw(Transform& transform) = 0;

    virtual bool OnMouseDown(int screenX, int screenY, Vector3 rayOrigin, Vector3 rayDirection) = 0;
    virtual void OnMouseMove(int screenX, int screenY) = 0;
    virtual void OnMouseUp() = 0;

    virtual bool IsActive();

protected:
    SceneViewWidget*    m_parent;
    bool                m_active;
};