#pragma once

#include "tinyxml2.h"
#include "Math/Algebra.h"

class GameObjectBase;

class Collider
{
public:
    enum ColliderType { SPHERE_COLLIDER, BOX_COLLIDER };

    Collider(GameObjectBase* gameObject);

    static Collider*        LoadFromXML(GameObjectBase* gameObject, tinyxml2::XMLElement* xml);
    static void             AddToGameObject(GameObjectBase* gameObject, ColliderType type);

    virtual void            Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc) = 0;
    virtual ColliderType    GetType() = 0;

    bool                    IsStatic;
    GameObjectBase*         GameObject;
};

class SphereCollider : public Collider
{
public:
    SphereCollider(GameObjectBase* gameObject, float radius = 1.0f);

    virtual void            Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);
    virtual ColliderType    GetType();

    float                   Radius;
};

class BoxCollider : public Collider
{
public:
    BoxCollider(GameObjectBase* gameObject);

    virtual void            Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);
    virtual ColliderType    GetType();

    Vector3                 MinPoint;
    Vector3                 MaxPoint;
};