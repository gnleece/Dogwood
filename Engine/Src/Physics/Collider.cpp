#include "Physics/Collider.h"

#include "GameObjectBase.h"
#include "Util.h"

using namespace tinyxml2;

Collider* Collider::LoadFromXML(XMLElement* xml)
{
    Collider::ColliderType type = (Collider::ColliderType)xml->IntAttribute("Type");

    switch(type)
    {
    case SPHERE_COLLIDER:
    {
        SphereCollider* collider = new SphereCollider();
        collider->Radius = xml->FloatAttribute("Radius");
        return collider;
    }
    case BOX_COLLIDER:
    {
        BoxCollider* collider = new BoxCollider();
        collider->MinPoint = ReadVector3FromXML(xml->FirstChildElement("MinPoint"));
        collider->MaxPoint = ReadVector3FromXML(xml->FirstChildElement("MaxPoint"));
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

Collider::ColliderType SphereCollider::GetType()
{
    return Collider::SPHERE_COLLIDER;
}

//------------------------------------------------------------------------------------

void BoxCollider::Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc)
{
    XMLElement* node = rootDoc.NewElement("Collider");
    parentNode->InsertEndChild(node);
    node->SetAttribute("Type", Collider::BOX_COLLIDER);
    node->InsertEndChild(WriteVector3ToXML(MinPoint, "MinPoint", rootDoc));
    node->InsertEndChild(WriteVector3ToXML(MaxPoint, "MaxPoint", rootDoc));
}

Collider::ColliderType BoxCollider::GetType()
{
    return Collider::BOX_COLLIDER;
}