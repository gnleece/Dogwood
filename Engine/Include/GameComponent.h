#pragma once

#include "GameObject.h"

class GameComponent
{
public:
    virtual void Start() {}
    virtual void Update(float /*deltaTime*/) {}     // TODO implement delta time

    virtual void OnEnable() {}
    virtual void OnDisable() {}

    void SetGameObject(GameObject* gameObject);

protected:
    GameObject* m_gameObject = NULL;
};