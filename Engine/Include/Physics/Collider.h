#pragma once

#include "tinyxml2.h"
#include "Math/Algebra.h"
#include "Math/Transform.h"

class GameObjectBase;

class Collider
{
public:
    enum ColliderType { SPHERE_COLLIDER, BOX_COLLIDER };

    Collider(GameObjectBase* gameObject);

    static Collider*        LoadFromXML(GameObjectBase* gameObject, tinyxml2::XMLElement* xml);
    static void             AddToGameObject(GameObjectBase* gameObject, ColliderType type);

    Transform&              GetTransform();

    virtual void            Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc) = 0;
    virtual ColliderType    GetType() = 0;
    virtual float           GetBoundingRadius() = 0;

    bool                    IsStatic;
    GameObjectBase*         GameObject;
};

class SphereCollider : public Collider
{
public:
    SphereCollider(GameObjectBase* gameObject, float radius = 1.0f);

    virtual void            Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);
    virtual ColliderType    GetType();
    virtual float           GetBoundingRadius();

    float                   Radius;
};

class BoxCollider : public Collider
{
public:
    BoxCollider(GameObjectBase* gameObject);

    virtual void            Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);
    virtual ColliderType    GetType();
    virtual float           GetBoundingRadius();

    Vector3                 MinPoint;
    Vector3                 MaxPoint;
};