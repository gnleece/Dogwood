#pragma once

class GameObject;

class GameComponent
{
public:
    GameComponent();

    // Public interface
    virtual void    OnCreate() {}
    virtual void    OnDestroy() {}

    virtual void    OnStart() {}

    virtual void    Update(float) {}

    virtual void    OnEnable() {}
    virtual void    OnDisable() {}

    // Engine use only - TODO enforce this
    GameObject*     GetGameObject();
    void            SetGameObject(GameObject* gameObject);

protected:
    GameObject*     m_gameObject;

    //bool          m_enabled;              // TODO implement me
};