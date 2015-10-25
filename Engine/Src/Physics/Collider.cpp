#include "Physics/Collider.h"

#include "GameObjectBase.h"

using namespace tinyxml2;

Collider* Collider::LoadFromXML(XMLElement* xml)
{
    Collider::ColliderType type = (Collider::ColliderType)xml->IntAttribute("Type");

    switch(type)
    {
    case SPHERE_COLLIDER:
    {
        float radius = xml->FloatAttribute("Radius");
        SphereCollider* collider = new SphereCollider();
        collider->Radius = radius;
        return collider;
    }
    case BOX_COLLIDER:
    {
        BoxCollider* collider = new BoxCollider();
        return collider;
    }
    }

    return NULL;
}

void Collider::AddToGameObject(GameObjectBase* gameObject, ColliderType type)
{
    Collider* collider = NULL;
    switch (type)
    {
    case SPHERE_COLLIDER:
        collider = new SphereCollider();
        break;
    case BOX_COLLIDER:
        collider = new BoxCollider();
        break;
    }

    if (collider != NULL)
    {
        gameObject->AddCollider(collider);
    }
}

//------------------------------------------------------------------------------------

SphereCollider::SphereCollider(float radius) : Radius(radius)
{
}

void SphereCollider::Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc)
{
    XMLElement* node = rootDoc.NewElement("Collider");
    parentNode->InsertEndChild(node);
    node->SetAttribute("Type", Collider::SPHERE_COLLIDER);
    node->SetAttribute("Radius", Radius);
}

//------------------------------------------------------------------------------------

void BoxCollider::Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc)
{
    XMLElement* node = rootDoc.NewElement("Collider");
    parentNode->InsertEndChild(node);
    node->SetAttribute("Type", Collider::BOX_COLLIDER);
}