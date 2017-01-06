#include "Physics/Collider.h"

#include "Debugging/DebugDraw.h"
#include "Math/Transformations.h"
#include "Serialization/HierarchicalSerializer.h"
#include "GameObjectBase.h"
#include "Util.h"

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

Collider* Collider::Load(HierarchicalDeserializer* deserializer, GameObjectBase* gameObject)
{
    Collider::ColliderType type;
    deserializer->GetAttribute("Type", (int&)type);
    Collider* collider = NULL;

    switch(type)
    {
    case SPHERE_COLLIDER:   collider = new SphereCollider(gameObject);  break;
    case BOX_COLLIDER:      collider = new BoxCollider(gameObject);     break;
    case CAPSULE_COLLIDER:  collider = new CapsuleCollider(gameObject); break;
    }

    if (collider != NULL)
    {
        collider->Load(deserializer);
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

void SphereCollider::Save(HierarchicalSerializer* serializer)
{
    serializer->PushScope("Collider");
    serializer->SetAttribute("Type", Collider::SPHERE_COLLIDER);
    serializer->SetAttribute("IsStatic", m_isStatic);
    serializer->SetAttribute("Radius", m_radius);
    serializer->InsertLeafVector3("Center", m_center);
    serializer->PopScope();
}

void SphereCollider::Load(HierarchicalDeserializer* deserializer)
{
    bool isStatic;
    deserializer->GetAttribute("IsStatic", isStatic);
    SetStatic(isStatic);

    Vector3 center;
    deserializer->ReadLeafVector3("Center", center);
    SetCenter(center);

    float radius;
    deserializer->GetAttribute("Radius", radius);
    SetLocalRadius(radius);
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

Matrix3x3 SphereCollider::GetInertiaTensor(float mass)
{
    return InertiaTensorSphere(m_radius, mass);
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

void BoxCollider::Save(HierarchicalSerializer* serializer)
{
    serializer->PushScope("Collider");
    serializer->SetAttribute("Type", Collider::BOX_COLLIDER);
    serializer->SetAttribute("IsStatic", m_isStatic);
    serializer->InsertLeafVector3("Center", m_center);
    serializer->InsertLeafVector3("Size", m_size);
    serializer->PopScope();
}

void BoxCollider::Load(HierarchicalDeserializer* deserializer)
{
    bool isStatic;
    deserializer->GetAttribute("IsStatic", isStatic);
    SetStatic(isStatic);

    Vector3 center;
    deserializer->ReadLeafVector3("Center", center);
    SetCenter(center);

    Vector3 size;
    deserializer->ReadLeafVector3("Size", size);
    SetLocalSize(size);
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

Matrix3x3 BoxCollider::GetInertiaTensor(float mass)
{
    return InertiaTensorCuboid(m_size, mass);       // TODO not sure if this is correct
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

void CapsuleCollider::Save(HierarchicalSerializer* serializer)
{
    serializer->PushScope("Collider");
    serializer->SetAttribute("Type", Collider::CAPSULE_COLLIDER);
    serializer->SetAttribute("IsStatic", m_isStatic);
    serializer->SetAttribute("Radius", m_radius);
    serializer->SetAttribute("Height", m_height);
    serializer->SetAttribute("Axis", m_axis);
    serializer->InsertLeafVector3("Center", m_center);
    serializer->PopScope();
}

void CapsuleCollider::Load(HierarchicalDeserializer* deserializer)
{
    bool isStatic;
    deserializer->GetAttribute("IsStatic", isStatic);
    SetStatic(isStatic);

    Vector3 center;
    deserializer->ReadLeafVector3("Center", center);
    SetCenter(center);

    float radius;
    deserializer->GetAttribute("Radius", radius);
    SetLocalRadius(radius);

    float height;
    deserializer->GetAttribute("Height", height);
    SetLocalHeight(height);

    int axis;
    deserializer->GetAttribute("Axis", axis);
    SetAxis((eAXIS)axis);
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

Matrix3x3 CapsuleCollider::GetInertiaTensor(float mass)
{
    // TODO implement for real
    return Matrix3x3::Identity;
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
    float maxRadius = sqrtf(a*a + b*b);
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