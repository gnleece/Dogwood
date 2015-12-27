#include "Physics/Collider.h"

#include "Debugging/DebugDraw.h"
#include "Math/Transformations.h"
#include "GameObjectBase.h"
#include "Util.h"

using namespace tinyxml2;

Collider::Collider(GameObjectBase* gameObject)
    : m_isStatic(true), m_gameObject(gameObject), m_center(Vector3::Zero)
{
    if (m_gameObject != NULL)
    {
        m_transform.SetParent(&(m_gameObject->GetTransform()));
    }
}

Collider::~Collider()
{
    m_transform.SetParent(NULL);
}

Collider* Collider::LoadFromXML(GameObjectBase* gameObject, XMLElement* xml)
{
    Collider::ColliderType type = (Collider::ColliderType)xml->IntAttribute("Type");
    Collider* collider = NULL;

    switch(type)
    {
    case SPHERE_COLLIDER:   collider = new SphereCollider(gameObject);  break;
    case BOX_COLLIDER:      collider = new BoxCollider(gameObject);     break;
    case CAPSULE_COLLIDER:  collider = new CapsuleCollider(gameObject); break;
    }

    if (collider != NULL)
    {
        collider->LoadFromXML(xml);
    }

    return collider;
}

void Collider::AddToGameObject(GameObjectBase* gameObject, ColliderType type)
{
    Collider* collider = NULL;

    switch (type)
    {
    case SPHERE_COLLIDER:   collider = new SphereCollider(gameObject);  break;
    case BOX_COLLIDER:      collider = new BoxCollider(gameObject);     break;
    case CAPSULE_COLLIDER:  collider = new CapsuleCollider(gameObject); break;
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
    return m_transform.GetWorldPosition();
}

bool Collider::IsStatic()
{
    return m_isStatic;
}

GameObjectBase* Collider::GetGameObject()
{
    return m_gameObject;
}

Transform& Collider::GetTransform()
{
    return m_transform;
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
    m_transform.SetLocalPosition(m_center);
}

//------------------------------------------------------------------------------------

SphereCollider::SphereCollider(GameObjectBase* gameObject, float radius)
    : Collider(gameObject), m_radius(radius)
{
}

void SphereCollider::LoadFromXML(XMLElement* xml)
{
    SetStatic(xml->BoolAttribute("IsStatic"));
    SetCenter(ReadVector3FromXML(xml->FirstChildElement("Center")));
    SetLocalRadius(xml->FloatAttribute("Radius"));
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

float SphereCollider::GetWorldspaceBoundingRadius()
{
    float scale = m_transform.GetWorldScale().MaxElement();
    return m_radius * scale;
}

void SphereCollider::DebugDraw(ColorRGB color, bool useDepth)
{
    Matrix4x4 m = Translation(GetWorldPosition());
    m = m * UniformScaling(GetWorldspaceBoundingRadius());
    DebugDraw::Singleton().DrawSphere(m, color, useDepth);
}

float SphereCollider::GetLocalRadius()
{
    return m_radius;
}

void SphereCollider::SetLocalRadius(float radius)
{
    m_radius = radius;
}

//------------------------------------------------------------------------------------

BoxCollider::BoxCollider(GameObjectBase* gameObject)
    : Collider(gameObject), m_size(Vector3::One)
{
}

void BoxCollider::LoadFromXML(XMLElement* xml)
{
    SetStatic(xml->BoolAttribute("IsStatic"));
    SetCenter(ReadVector3FromXML(xml->FirstChildElement("Center")));
    SetLocalSize(ReadVector3FromXML(xml->FirstChildElement("Size")));
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

float BoxCollider::GetWorldspaceBoundingRadius()
{
    Vector3 worldScale = m_transform.TransformVector(m_size);
    return worldScale.Magnitude();
}

void BoxCollider::DebugDraw(ColorRGB color, bool useDepth)
{
    Matrix4x4 m = m_transform.GetWorldMatrix() * Scaling(m_size);
    DebugDraw::Singleton().DrawCube(m, color, useDepth);
}

Vector3 BoxCollider::GetLocalSize()
{
    return m_size;
}

void BoxCollider::SetLocalSize(Vector3 size)
{
    m_size = size;
}

Vector3 BoxCollider::GetWorldScaleHalfsize()
{
    return (Scaling(m_transform.GetWorldScale()) * Vector4(m_size, 0)).xyz();
}

//------------------------------------------------------------------------------------

CapsuleCollider::CapsuleCollider(GameObjectBase* gameObject, float radius, float height, eAXIS axis)
    : Collider(gameObject), m_radius(radius), m_height(height), m_axis(axis)
{
}

CapsuleCollider::~CapsuleCollider()
{
    if (m_debugCapsule != NULL)
    {
        delete m_debugCapsule;
    }
}

void CapsuleCollider::LoadFromXML(XMLElement* xml)
{
    SetStatic(xml->BoolAttribute("IsStatic"));
    SetCenter(ReadVector3FromXML(xml->FirstChildElement("Center")));
    SetLocalRadius(xml->FloatAttribute("Radius"));
    SetLocalHeight(xml->FloatAttribute("Height"));
    SetAxis((eAXIS)xml->IntAttribute("Axis"));
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
    node->SetAttribute("Axis", m_axis);
}

Collider::ColliderType CapsuleCollider::GetType()
{
    return Collider::CAPSULE_COLLIDER;
}

float CapsuleCollider::GetWorldspaceBoundingRadius()
{
    float worldHeight = CalculateWorldHeight();
    float worldRadius = CalculateWorldRadius();
    return worldHeight/2 + worldRadius;
}

void CapsuleCollider::DebugDraw(ColorRGB color, bool useDepth)
{
    if (m_debugCapsule == NULL)
    {
        RefreshDebugInfo();
    }

    Matrix4x4 r = RotationEulerAngles(m_transform.GetWorldRotation());
    Matrix4x4 m = Translation(GetWorldPosition()) * r;
    m_debugCapsule->Draw(m, color, useDepth);
}

float CapsuleCollider::GetLocalRadius()
{
    return m_radius;
}

float CapsuleCollider::GetLocalHeight()
{
    return m_height;
}

eAXIS CapsuleCollider::GetAxis()
{
    return m_axis;
}

void CapsuleCollider::SetLocalRadius(float radius)
{
    m_radius = radius;
    RefreshDebugInfo();
}

void CapsuleCollider::SetLocalHeight(float height)
{
    m_height = height;
    RefreshDebugInfo();
}

void CapsuleCollider::SetAxis(eAXIS axis)
{
    if (axis < AXIS_X || axis > AXIS_Z)
        axis = AXIS_Y;

    m_axis = axis;
    RefreshDebugInfo();
}

float CapsuleCollider::CalculateWorldRadius()
{
    Vector3 objectWorldScale = m_transform.GetWorldScale();
    float a = objectWorldScale[(m_axis + 1) % 3];
    float b = objectWorldScale[(m_axis + 2) % 3];
    float maxRadius = sqrt(a*a + b*b);
    float worldRadius = m_radius * maxRadius;
    return worldRadius;
}

float CapsuleCollider::CalculateWorldHeight()
{
    Vector3 objectWorldScale = m_transform.GetWorldScale();
    float worldHeight = m_height * objectWorldScale[m_axis];
    return worldHeight;
}

void CapsuleCollider::RefreshDebugInfo()
{
    // TODO #define this out for non debug builds
    if (m_debugCapsule != NULL)
    {
        delete m_debugCapsule;
    }

    float worldRadius = CalculateWorldRadius();
    float worldHeight = CalculateWorldHeight();

    m_debugCapsule = new DebugCapsule();
    m_debugCapsule->Init(worldRadius, worldHeight, 10, m_axis);
}