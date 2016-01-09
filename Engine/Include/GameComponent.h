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

    virtual void    OnCollisionEnter() {}
    virtual void    OnCollisionHold() {}
    virtual void    OnCollisionExit() {}

    bool            IsEnabled();

    // Engine use only - TODO enforce this
    GameObject*     GetGameObject();
    void            SetGameObject(GameObject* gameObject);

protected:
    void            SetEnabled(bool enabled);

    GameObject*     m_gameObject;

private:
    bool            m_enabled;
};