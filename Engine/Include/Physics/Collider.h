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

    Transform&              GetTransform();

    virtual void            Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc) = 0;
    virtual ColliderType    GetType() = 0;
    virtual float           GetBoundingRadius() = 0;

    virtual void            DebugDraw(ColorRGB color);

    bool                    IsStatic;
    GameObjectBase*         GameObject;
    Vector3                 Center;
};

class SphereCollider : public Collider
{
public:
    SphereCollider(GameObjectBase* gameObject, float radius = 1.0f);

    virtual void            Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);
    virtual ColliderType    GetType();
    virtual float           GetBoundingRadius();

    virtual void            DebugDraw(ColorRGB color);

    float                   Radius;
};

class BoxCollider : public Collider
{
public:
    BoxCollider(GameObjectBase* gameObject);

    virtual void            Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);
    virtual ColliderType    GetType();
    virtual float           GetBoundingRadius();

    virtual void            DebugDraw(ColorRGB color);

    Vector3                 Size;
};

class CapsuleCollider : public Collider
{
public:
    CapsuleCollider(GameObjectBase* gameObject, float radius = 1, float height = 2);
    ~CapsuleCollider();

    virtual void            Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);
    virtual ColliderType    GetType();
    virtual float           GetBoundingRadius();

    virtual void            DebugDraw(ColorRGB color);

    float                   Radius;
    float                   Height;

private:
    DebugCapsule*           m_debugCapsule;
};