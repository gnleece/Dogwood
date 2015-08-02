#pragma once

#include "GameObject.h"

class GameComponent
{
public:

    // Public interface
    virtual void    OnCreate() {}
    virtual void    OnDestroy() {}

    virtual void    OnStart() {}

    virtual void    Update(float) {}
    virtual void    Render(Transform& /*transform*/, bool /*wireframe*/) {}

    virtual void    OnEnable() {}
    virtual void    OnDisable() {}

    // Engine use only - TODO enforce this
    void            SetGameObject(GameObject* gameObject);

protected:
    GameObject*     m_gameObject = NULL;
};