#pragma once

#include "tinyxml2.h"

class GameObjectBase;

class Collider
{
public:
    enum ColliderType { SPHERE_COLLIDER, BOX_COLLIDER };

    static Collider*    LoadFromXML(tinyxml2::XMLElement* xml);
    static void         AddToGameObject(GameObjectBase* gameObject, ColliderType type);

    virtual void        Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc) = 0;
};

class SphereCollider : public Collider
{
public:
    SphereCollider(float radius = 1);

    virtual void        Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);

    float Radius;
};

class BoxCollider : public Collider
{
public:
    virtual void        Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc);
};