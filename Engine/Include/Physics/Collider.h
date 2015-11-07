#pragma once

#include "tinyxml2.h"
#include "Math/Algebra.h"

class GameObjectBase;

class Collider
{
public:
    enum ColliderType { SPHERE_COLLIDER, BOX_COLLIDER };

    Collider();

    static Collider*        LoadFromXML(tinyxml2::XMLElement* xml);
    static void             AddToGameObject(GameObjectBase* gameObject, ColliderType type);

    virtual void            Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc) = 0;
    virtual ColliderType    GetType() = 0;

    bool                    IsStatic;
};

class SphereCollider : public Collider
{
public:
    SphereCollider(float radius = 1.0f);

    virtual void            Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);
    virtual ColliderType    GetType();

    float                   Radius;
};

class BoxCollider : public Collider
{
public:
    BoxCollider();

    virtual void            Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);
    virtual ColliderType    GetType();

    Vector3                 MinPoint;
    Vector3                 MaxPoint;
};