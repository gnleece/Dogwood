#pragma once

#include "tinyxml2.h"
#include "Math/Algebra.h"
#include "Math/Transform.h"
#include "Rendering/Color.h"

class DebugCapsule;
class GameObjectBase;

class Collider
{
public:
    enum ColliderType { SPHERE_COLLIDER, BOX_COLLIDER, CAPSULE_COLLIDER };

    Collider(GameObjectBase* gameObject);

    static Collider*        LoadFromXML(GameObjectBase* gameObject, tinyxml2::XMLElement* xml);
    static void             AddToGameObject(GameObjectBase* gameObject, ColliderType type);

    virtual void            Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc) = 0;
    virtual ColliderType    GetType() = 0;
    virtual float           GetBoundingRadius() = 0;
    virtual void            DebugDraw(ColorRGB color, bool useDepth = true);

    Vector3                 GetWorldPosition();

    bool                    IsStatic();
    GameObjectBase*         GetGameObject();
    Vector3                 GetCenter();

    void                    SetStatic(bool isStatic);
    void                    SetCenter(Vector3 center);

protected:
    bool                    m_isStatic;
    GameObjectBase*         m_gameObject;
    Vector3                 m_center;
};

class SphereCollider : public Collider
{
public:
    SphereCollider(GameObjectBase* gameObject, float radius = 1.0f);

    virtual void            Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);
    virtual ColliderType    GetType();
    virtual float           GetBoundingRadius();
    virtual void            DebugDraw(ColorRGB color, bool useDepth = true);

    float                   GetRadius();
    void                    SetRadius(float radius);

private:
    float                   m_radius;
};

class BoxCollider : public Collider
{
public:
    BoxCollider(GameObjectBase* gameObject);

    virtual void            Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);
    virtual ColliderType    GetType();
    virtual float           GetBoundingRadius();
    virtual void            DebugDraw(ColorRGB color, bool useDepth = true);

    Vector3                 GetSize();
    void                    SetSize(Vector3 size);

private:
    Vector3                 m_size;
};

class CapsuleCollider : public Collider
{
public:
    CapsuleCollider(GameObjectBase* gameObject, float radius = 1, float height = 2);
    ~CapsuleCollider();

    virtual void            Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);
    virtual ColliderType    GetType();
    virtual float           GetBoundingRadius();
    virtual void            DebugDraw(ColorRGB color, bool useDepth = true);

    float                   GetRadius();
    float                   GetHeight();

    void                    SetRadius(float radius);
    void                    SetHeight(float height);

private:
    void                    RefreshDebugInfo();

    float                   m_radius;
    float                   m_height;

    DebugCapsule*           m_debugCapsule;
};