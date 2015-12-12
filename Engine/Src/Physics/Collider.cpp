#include "Physics/Collider.h"

#include "Debugging/DebugDraw.h"
#include "Math/Transformations.h"
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
        collider->Center = ReadVector3FromXML(xml->FirstChildElement("Center"));
        collider->Radius = xml->FloatAttribute("Radius");
        return collider;
    }
    case BOX_COLLIDER:
    {
        BoxCollider* collider = new BoxCollider(gameObject);
        collider->IsStatic = xml->BoolAttribute("IsStatic");
        collider->Center = ReadVector3FromXML(xml->FirstChildElement("Center"));
        collider->Size = ReadVector3FromXML(xml->FirstChildElement("Size"));
        return collider;
    }
    case CAPSULE_COLLIDER:
    {
        CapsuleCollider* collider = new CapsuleCollider(gameObject);
        collider->IsStatic = xml->BoolAttribute("IsStatic");
        collider->Center = ReadVector3FromXML(xml->FirstChildElement("Center"));
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

void Collider::DebugDraw(ColorRGB color)
{
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
    node->InsertEndChild(WriteVector3ToXML(Center, "Center", rootDoc));
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

void SphereCollider::DebugDraw(ColorRGB color)
{
    Vector3 position = GetTransform().GetWorldPosition();
    Matrix4x4 m = Translation(position);
    m = m * UniformScaling(Radius);
    DebugDraw::Singleton().DrawSphere(m, color);
}

//------------------------------------------------------------------------------------

BoxCollider::BoxCollider(GameObjectBase* gameObject) : Collider(gameObject)
{
    Size = Vector3::One;
}

void BoxCollider::Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc)
{
    XMLElement* node = rootDoc.NewElement("Collider");
    parentNode->InsertEndChild(node);
    node->SetAttribute("Type", Collider::BOX_COLLIDER);
    node->SetAttribute("IsStatic", IsStatic);
    node->InsertEndChild(WriteVector3ToXML(Center, "Center", rootDoc));
    node->InsertEndChild(WriteVector3ToXML(Size, "Size", rootDoc));
}

Collider::ColliderType BoxCollider::GetType()
{
    return Collider::BOX_COLLIDER;
}

float BoxCollider::GetBoundingRadius()
{
    return 1;       // TODO implement me!
}

void BoxCollider::DebugDraw(ColorRGB color)
{
    // TODO implement me
}

//------------------------------------------------------------------------------------

CapsuleCollider::CapsuleCollider(GameObjectBase* gameObject, float radius, float height) 
    : Collider(gameObject), Radius(radius), Height(height)
{
}

CapsuleCollider::~CapsuleCollider()
{
    if (m_debugCapsule != NULL)
    {
        delete m_debugCapsule;
    }
}

void CapsuleCollider::Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc)
{
    XMLElement* node = rootDoc.NewElement("Collider");
    parentNode->InsertEndChild(node);
    node->SetAttribute("Type", Collider::CAPSULE_COLLIDER);
    node->SetAttribute("IsStatic", IsStatic);
    node->InsertEndChild(WriteVector3ToXML(Center, "Center", rootDoc));
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

void CapsuleCollider::DebugDraw(ColorRGB color)
{
    if (m_debugCapsule == NULL)
    {
        m_debugCapsule = new DebugCapsule();
        m_debugCapsule->Init(Radius, Height, 10);
    }

    Vector3 position = GetTransform().GetWorldPosition();
    Matrix4x4 m = Translation(position);
    m_debugCapsule->Draw(m, color);
}