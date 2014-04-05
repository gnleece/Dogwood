#pragma once

class GameComponent
{
public:
    virtual void Start();
    virtual void Update(float deltaTime);

    virtual void OnEnable();
    virtual void OnDisable();
};