#include "Math\Transform.h"
#include "Math\Transformations.h"

Transform Transform::Identity = Transform();

Transform::Transform()
{
    m_position  = Vector3(0.0f, 0.0f, 0.0f);
    m_rotation  = Vector3(0.0f, 0.0f, 0.0f);
    m_scale     = Vector3(1.0f, 1.0f, 1.0f);
    ComputeMatrixFromComponents();
    m_changed = true;
}

Transform::Transform(Matrix4x4& m)
{
    SetMatrix(m);
}

void Transform::SetMatrix(Matrix4x4& m)
{
    m_matrix = m;
    DecomposeMatrix(m_matrix, m_position, m_rotation, m_scale);
    CalculateDirectionVectors();
    m_dirty = false;
    m_changed = true;
}

void Transform::SetPosition(Vector3& position)
{
    m_position = position;
    m_dirty = true;
    m_changed = true;
}

void Transform::SetRotation(Vector3& rotation)
{
    m_rotation = rotation;
    m_dirty = true;
    m_changed = true;
}

void Transform::SetScale(Vector3& scale)
{
    m_scale = scale;
    m_dirty = true;
    m_changed = true;
}

Matrix4x4& Transform::GetMatrix()
{
    if (m_dirty)
    {
        ComputeMatrixFromComponents();
    }

    return m_matrix;
}

Vector3& Transform::GetPosition()
{
    return m_position;
}

Vector3& Transform::GetRotation()
{
    return m_rotation;
}

Vector3& Transform::GetScale()
{
    return m_scale;
}

Vector3& Transform::GetRight()
{
    if (m_dirty)
    {
        ComputeMatrixFromComponents();
    }
    return m_right;
}

Vector3& Transform::GetUp()
{
    if (m_dirty)
    {
        ComputeMatrixFromComponents();
    }
    return m_up;
}

Vector3& Transform::GetForward()
{
    if (m_dirty)
    {
        ComputeMatrixFromComponents();
    }
    return m_forward;
}

bool Transform::IsDirty()
{
    return m_dirty;
}

bool Transform::HasChanged()
{
    return m_changed;
}

void Transform::ClearChangedFlag()
{
    m_changed = false;
}

void Transform::ComputeMatrixFromComponents()
{
    // Combine: T*R*S
    m_matrix = Translation(m_position);
    m_matrix = m_matrix*RotationEulerAngles(m_rotation);
    m_matrix = m_matrix*Scaling(m_scale);

    CalculateDirectionVectors();

    m_dirty = false;
}

void Transform::CalculateDirectionVectors()
{
    m_right     = (m_matrix*Vector4(Vector3::Right, 0)).xyz().Normalized();
    m_up        = (m_matrix*Vector4(Vector3::Up, 0)).xyz().Normalized();
    m_forward   = (m_matrix*Vector4(Vector3::Forward, 0)).xyz().Normalized();
}

Transform operator *(Transform& a, Transform& b)
{
    // TODO clean up
    if (a.IsDirty())
    {
        a.ComputeMatrixFromComponents();
    }
    if (b.IsDirty())
    {
        b.ComputeMatrixFromComponents();
    }
    
    Transform ret;
    ret.SetMatrix(a.GetMatrix() * b.GetMatrix());
    return ret;
}

Vector4 operator *(Transform& m, Vector4& v)
{
    return m.GetMatrix() * v;
}