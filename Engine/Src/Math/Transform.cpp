#include "Math\Transform.h"
#include "Math\Transformations.h"

#include <algorithm>

Transform Transform::Identity = Transform();

Transform::Transform()
{
    m_parent = NULL;
    m_localPosition  = Vector3(0.0f, 0.0f, 0.0f);
    m_localRotation  = Vector3(0.0f, 0.0f, 0.0f);
    m_localScale     = Vector3(1.0f, 1.0f, 1.0f);
    m_recomputeLocal = true;
    m_recomputeWorld = true;
}

Transform::Transform(Matrix4x4& m)
{
    m_parent = NULL;
    SetLocalMatrix(m);
}

Transform::~Transform()
{
    if (m_parent != NULL)
    {
        m_parent->RemoveChild(this);
    }
}

void Transform::SetParent(Transform* parent)
{
    if (m_parent != NULL)
    {
        m_parent->RemoveChild(this);
    }

    m_parent = parent;
    if (m_parent != NULL)
    {
        m_parent->m_children.push_back(this);
    }

    SetRecomputeWorldFlag();
}

void Transform::SetLocalMatrix(Matrix4x4& m)
{
    m_localMatrix = m;
    DecomposeMatrix(m_localMatrix, m_localPosition, m_localRotation, m_localScale);
    m_recomputeLocal = false;
    SetRecomputeWorldFlag();
}

void Transform::SetLocalPosition(Vector3& position)
{
    m_localPosition = position;
    m_recomputeLocal = true;
    SetRecomputeWorldFlag();
}

void Transform::SetLocalRotation(Vector3& rotation)
{
    m_localRotation = rotation;
    m_recomputeLocal = true;
    SetRecomputeWorldFlag();
}

void Transform::SetLocalScale(Vector3& scale)
{
    m_localScale = scale;
    m_recomputeLocal = true;
    SetRecomputeWorldFlag();
}

void Transform::SetLocalVector(Vector3& vector, TransformVectorType type)
{
    switch (type)
    {
    case eVector_Position:  SetLocalPosition(vector);    break;
    case eVector_Rotation:  SetLocalRotation(vector);    break;
    case eVector_Scale:     SetLocalScale(vector);       break;
    }
}

Matrix4x4& Transform::GetLocalMatrix()
{
    RecomputeLocalIfDirty();
    return m_localMatrix;
}

Vector3& Transform::GetLocalPosition()
{
    RecomputeLocalIfDirty();
    return m_localPosition;
}

Vector3& Transform::GetLocalRotation()
{
    RecomputeLocalIfDirty();
    return m_localRotation;
}

Vector3& Transform::GetLocalScale()
{
    RecomputeLocalIfDirty();
    return m_localScale;
}

Vector3& Transform::GetLocalVector(TransformVectorType type)
{
    switch (type)
    {
    case eVector_Position:  return GetLocalPosition();
    case eVector_Rotation:  return GetLocalRotation();
    case eVector_Scale:     return GetLocalScale();
    }
    return Vector3::Zero;
}

Matrix4x4& Transform::GetWorldMatrix()
{
    RecomputeWorldIfDirty();
    return m_worldMatrix;
}

Vector3& Transform::GetWorldPosition()
{
    RecomputeLocalIfDirty();
    RecomputeWorldIfDirty();
    return m_worldPosition;
}

Vector3& Transform::GetWorldRotation()
{
    RecomputeLocalIfDirty();
    RecomputeWorldIfDirty();
    return m_worldRotation;
}

Vector3& Transform::GetWorldScale()
{
    RecomputeLocalIfDirty();
    RecomputeWorldIfDirty();
    return m_worldScale;
}

Vector3& Transform::GetWorldVector(TransformVectorType type)
{
    switch (type)
    {
    case eVector_Position:  return GetWorldPosition();
    case eVector_Rotation:  return GetWorldRotation();
    case eVector_Scale:     return GetWorldScale();
    }
    return Vector3::Zero;
}

Vector3& Transform::GetRight()
{
    RecomputeWorldIfDirty();
    return m_right;
}

Vector3& Transform::GetUp()
{
    RecomputeWorldIfDirty();
    return m_up;
}

Vector3& Transform::GetForward()
{
    RecomputeWorldIfDirty();
    return m_forward;
}

void Transform::RemoveChild(Transform* transform)
{
    m_children.erase(
        std::remove(m_children.begin(), m_children.end(), transform),
        m_children.end());
}

void Transform::SetRecomputeWorldFlag()
{
    m_recomputeWorld = true;

    vector<Transform*>::iterator iter;
    for (iter = m_children.begin(); iter != m_children.end(); iter++)
    {
        (*iter)->SetRecomputeWorldFlag();
    }
}

void Transform::ClearRecomputeWorldFlag()
{
    m_recomputeWorld = false;
}

void Transform::RecomputeLocalIfDirty()
{
    if (m_recomputeLocal)
    {
        // Combine: T*R*S
        m_localMatrix = Translation(m_localPosition);
        m_localMatrix = m_localMatrix*RotationEulerAngles(m_localRotation);
        m_localMatrix = m_localMatrix*Scaling(m_localScale);

        m_recomputeLocal = false;
    }
}

void Transform::RecomputeWorldIfDirty()
{
    if (m_recomputeWorld)
    {
        RecomputeLocalIfDirty();
        Matrix4x4 parentTransform = m_parent ? m_parent->GetWorldMatrix() : Matrix4x4::Identity;
        m_worldMatrix = parentTransform * m_localMatrix;
        CalculateDirectionVectors();
        ClearRecomputeWorldFlag();
    }
}

void Transform::CalculateDirectionVectors()
{
    m_right     = (m_worldMatrix*Vector4(Vector3::Right,     0)).xyz().Normalized();
    m_up        = (m_worldMatrix*Vector4(Vector3::Up,        0)).xyz().Normalized();
    m_forward   = (m_worldMatrix*Vector4(Vector3::Forward,   0)).xyz().Normalized();
}

Transform operator *(Transform& a, Transform& b)
{
    Transform ret;
    ret.SetLocalMatrix(a.GetWorldMatrix() * b.GetWorldMatrix());
    return ret;
}

Vector4 operator *(Transform& m, Vector4& v)
{
    return m.GetWorldMatrix() * v;
}