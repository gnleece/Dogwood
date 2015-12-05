#include "Physics/Collider.h"

#include "GameObjectBase.h"
#include "Util.h"

using namespace tinyxml2;

Collider::Collider(GameObjectBase* gameObject) : IsStatic(true), GameObject(gameObject)
{
}

Collider* Collider::LoadFromXML(GameObjectBase* gameObject, XMLElement* xml)
{
    Collider::ColliderType type = (Collider::ColliderType)xml->IntAttribute("Type");

    switch(type)
    {
    case SPHERE_COLLIDER:
    {
        SphereCollider* collider = new SphereCollider(gameObject);
        collider->IsStatic = xml->BoolAttribute("IsStatic");
        collider->Radius = xml->FloatAttribute("Radius");
        return collider;
    }
    case BOX_COLLIDER:
    {
        BoxCollider* collider = new BoxCollider(gameObject);
        collider->IsStatic = xml->BoolAttribute("IsStatic");
        collider->MinPoint = ReadVector3FromXML(xml->FirstChildElement("MinPoint"));
        collider->MaxPoint = ReadVector3FromXML(xml->FirstChildElement("MaxPoint"));
        return collider;
    }
    case CAPSULE_COLLIDER:
    {
        CapsuleCollider* collider = new CapsuleCollider(gameObject);
        collider->IsStatic = xml->BoolAttribute("IsStatic");
        collider->Radius = xml->FloatAttribute("Radius");
        collider->Height = xml->FloatAttribute("Height");
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
        collider = new SphereCollider(gameObject);
        break;
    case BOX_COLLIDER:
        collider = new BoxCollider(gameObject);
        break;
    case CAPSULE_COLLIDER:
        collider = new CapsuleCollider(gameObject);
        break;
    }

    if (collider != NULL)
    {
        gameObject->AddCollider(collider);
    }
}

Transform& Collider::GetTransform()
{
    return GameObject->GetTransform();
}

//------------------------------------------------------------------------------------

SphereCollider::SphereCollider(GameObjectBase* gameObject, float radius) : Collider(gameObject), Radius(radius)
{
}

void SphereCollider::Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc)
{
    XMLElement* node = rootDoc.NewElement("Collider");
    parentNode->InsertEndChild(node);
    node->SetAttribute("Type", Collider::SPHERE_COLLIDER);
    node->SetAttribute("IsStatic", IsStatic);
    node->SetAttribute("Radius", Radius);
}

Collider::ColliderType SphereCollider::GetType()
{
    return Collider::SPHERE_COLLIDER;
}

float SphereCollider::GetBoundingRadius()
{
    return Radius;
}

//------------------------------------------------------------------------------------

BoxCollider::BoxCollider(GameObjectBase* gameObject) : Collider(gameObject)
{
}

void BoxCollider::Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc)
{
    XMLElement* node = rootDoc.NewElement("Collider");
    parentNode->InsertEndChild(node);
    node->SetAttribute("Type", Collider::BOX_COLLIDER);
    node->SetAttribute("IsStatic", IsStatic);
    node->InsertEndChild(WriteVector3ToXML(MinPoint, "MinPoint", rootDoc));
    node->InsertEndChild(WriteVector3ToXML(MaxPoint, "MaxPoint", rootDoc));
}

Collider::ColliderType BoxCollider::GetType()
{
    return Collider::BOX_COLLIDER;
}

float BoxCollider::GetBoundingRadius()
{
    return 1;       // TODO implement me!
}

//------------------------------------------------------------------------------------

CapsuleCollider::CapsuleCollider(GameObjectBase* gameObject, float radius, float height) 
    : Collider(gameObject), Radius(radius), Height(height)
{
}

void CapsuleCollider::Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc)
{
    XMLElement* node = rootDoc.NewElement("Collider");
    parentNode->InsertEndChild(node);
    node->SetAttribute("Type", Collider::CAPSULE_COLLIDER);
    node->SetAttribute("IsStatic", IsStatic);
    node->SetAttribute("Radius", Radius);
    node->SetAttribute("Height", Height);
}

Collider::ColliderType CapsuleCollider::GetType()
{
    return Collider::CAPSULE_COLLIDER;
}

float CapsuleCollider::GetBoundingRadius()
{
    return Height + 2*Radius;
}