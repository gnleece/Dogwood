#include "Physics/Collider.h"

#include "Debugging/DebugDraw.h"
#include "Math/Transformations.h"
#include "GameObjectBase.h"
#include "Util.h"

using namespace tinyxml2;

Collider::Collider(GameObjectBase* gameObject)
    : m_isStatic(true), m_gameObject(gameObject), m_center(Vector3::Zero)
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
        collider->SetStatic(xml->BoolAttribute("IsStatic"));
        collider->SetCenter(ReadVector3FromXML(xml->FirstChildElement("Center")));
        collider->SetRadius(xml->FloatAttribute("Radius"));
        return collider;
    }
    case BOX_COLLIDER:
    {
        BoxCollider* collider = new BoxCollider(gameObject);
        collider->SetStatic(xml->BoolAttribute("IsStatic"));
        collider->SetCenter(ReadVector3FromXML(xml->FirstChildElement("Center")));
        collider->SetSize(ReadVector3FromXML(xml->FirstChildElement("Size")));
        return collider;
    }
    case CAPSULE_COLLIDER:
    {
        CapsuleCollider* collider = new CapsuleCollider(gameObject);
        collider->SetStatic(xml->BoolAttribute("IsStatic"));
        collider->SetCenter(ReadVector3FromXML(xml->FirstChildElement("Center")));
        collider->SetRadius(xml->FloatAttribute("Radius"));
        collider->SetHeight(xml->FloatAttribute("Height"));
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

void Collider::DebugDraw(ColorRGB color, bool useDepth)
{
}

Vector3 Collider::GetWorldPosition()
{
    Transform t = m_gameObject->GetTransform();
    Vector3 offset = (t.GetWorldMatrix() * Vector4(m_center, 0)).xyz();
    Vector3 position = t.GetWorldPosition() + offset;
    return position;
}

bool Collider::IsStatic()
{
    return m_isStatic;
}

GameObjectBase* Collider::GetGameObject()
{
    return m_gameObject;
}

Vector3 Collider::GetCenter()
{
    return m_center;
}

void Collider::SetStatic(bool isStatic)
{
    m_isStatic = isStatic;
}

void Collider::SetCenter(Vector3 center)
{
    m_center = center;
}

//------------------------------------------------------------------------------------

SphereCollider::SphereCollider(GameObjectBase* gameObject, float radius)
    : Collider(gameObject), m_radius(radius)
{
}

void SphereCollider::Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc)
{
    XMLElement* node = rootDoc.NewElement("Collider");
    parentNode->InsertEndChild(node);
    node->SetAttribute("Type", Collider::SPHERE_COLLIDER);
    node->SetAttribute("IsStatic", m_isStatic);
    node->InsertEndChild(WriteVector3ToXML(m_center, "Center", rootDoc));
    node->SetAttribute("Radius", m_radius);
}

Collider::ColliderType SphereCollider::GetType()
{
    return Collider::SPHERE_COLLIDER;
}

float SphereCollider::GetBoundingRadius()
{
    float scale = m_gameObject->GetTransform().GetWorldScale().MaxElement();
    return m_radius * scale;
}

void SphereCollider::DebugDraw(ColorRGB color, bool useDepth)
{
    Matrix4x4 m = Translation(GetWorldPosition());
    m = m * UniformScaling(GetBoundingRadius());
    DebugDraw::Singleton().DrawSphere(m, color, useDepth);
}

float SphereCollider::GetRadius()
{
    return m_radius;
}

void SphereCollider::SetRadius(float radius)
{
    m_radius = radius;
}

//------------------------------------------------------------------------------------

BoxCollider::BoxCollider(GameObjectBase* gameObject)
    : Collider(gameObject), m_size(Vector3::One)
{
}

void BoxCollider::Serialize(tinyxml2::XMLNode* parentNode, tinyxml2::XMLDocument& rootDoc)
{
    XMLElement* node = rootDoc.NewElement("Collider");
    parentNode->InsertEndChild(node);
    node->SetAttribute("Type", Collider::BOX_COLLIDER);
    node->SetAttribute("IsStatic", m_isStatic);
    node->InsertEndChild(WriteVector3ToXML(m_center, "Center", rootDoc));
    node->InsertEndChild(WriteVector3ToXML(m_size, "Size", rootDoc));
}

Collider::ColliderType BoxCollider::GetType()
{
    return Collider::BOX_COLLIDER;
}

float BoxCollider::GetBoundingRadius()
{
    Matrix4x4 m = m_gameObject->GetTransform().GetWorldMatrix();
    Vector3 worldScale = (m * Vector4(m_size, 0)).xyz();
    return worldScale.Magnitude();
}

void BoxCollider::DebugDraw(ColorRGB color, bool useDepth)
{
    Matrix4x4 m = m_gameObject->GetTransform().GetWorldMatrix();
    m = m * Translation(m_center) * Scaling(m_size);
    DebugDraw::Singleton().DrawCube(m, color, useDepth);
}

Vector3 BoxCollider::GetSize()
{
    return m_size;
}

void BoxCollider::SetSize(Vector3 size)
{
    m_size = size;
}

//------------------------------------------------------------------------------------

CapsuleCollider::CapsuleCollider(GameObjectBase* gameObject, float radius, float height) 
    : Collider(gameObject), m_radius(radius), m_height(height)
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
    node->SetAttribute("IsStatic", m_isStatic);
    node->InsertEndChild(WriteVector3ToXML(m_center, "Center", rootDoc));
    node->SetAttribute("Radius", m_radius);
    node->SetAttribute("Height", m_height);
}

Collider::ColliderType CapsuleCollider::GetType()
{
    return Collider::CAPSULE_COLLIDER;
}

float CapsuleCollider::GetBoundingRadius()
{
    return m_height + 2*m_radius;
}

void CapsuleCollider::DebugDraw(ColorRGB color, bool useDepth)
{
    if (m_debugCapsule == NULL)
    {
        RefreshDebugInfo();
    }

    Matrix4x4 m = Translation(GetWorldPosition());
    m_debugCapsule->Draw(m, color, useDepth);
}

float CapsuleCollider::GetRadius()
{
    return m_radius;
}

float CapsuleCollider::GetHeight()
{
    return m_height;
}

void CapsuleCollider::SetRadius(float radius)
{
    m_radius = radius;
    RefreshDebugInfo();
}

void CapsuleCollider::SetHeight(float height)
{
    m_height = height;
    RefreshDebugInfo();
}

void CapsuleCollider::RefreshDebugInfo()
{
    // TODO #define this out for non debug builds
    if (m_debugCapsule != NULL)
    {
        delete m_debugCapsule;
    }

    m_debugCapsule = new DebugCapsule();
    m_debugCapsule->Init(m_radius, m_height, 10);
}