#pragma once

#include "GameObject.h"

class GameComponent
{
public:
    GameComponent(GameObject& gameObject);

    virtual void Start() {}
    virtual void Update(float deltaTime) {}

    virtual void OnEnable() {}
    virtual void OnDisable() {}

protected:
    GameObject m_gameObject;
};