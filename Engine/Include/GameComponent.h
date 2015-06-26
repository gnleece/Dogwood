#pragma once

#include "GameObject.h"

class GameComponent
{
public:
    virtual void OnCreate() {}
    virtual void OnDestroy() {}

    virtual void OnStart() {}

    virtual void Update(float) {}

    virtual void OnEnable() {}
    virtual void OnDisable() {}

    void SetGameObject(GameObject* gameObject);

protected:
    GameObject* m_gameObject = NULL;
};