#include "Math\Transform.h"
#include "Math\Transformations.h"

Transform Transform::Identity = Transform();

Transform::Transform()
{
    m_position  = Vector3(0.0f, 0.0f, 0.0f);
    m_rotation  = Vector3(0.0f, 0.0f, 0.0f);
    m_scale     = Vector3(1.0f, 1.0f, 1.0f);
    ComputeMatrixFromComponents();
}

void Transform::SetMatrix(Matrix4x4& m)
{
    m_matrix = m;
    DecomposeMatrix(m_matrix, m_position, m_rotation, m_scale);
    m_dirty = false;
}

void Transform::SetPosition(Vector3& position)
{
    m_position = position;
    m_dirty = true;
}

void Transform::SetRotation(Vector3& rotation)
{
    m_rotation = rotation;
    m_dirty = true;
}

void Transform::SetScale(Vector3& scale)
{
    m_scale = scale;
    m_dirty = true;
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

bool Transform::IsDirty()
{
    return m_dirty;
}

void Transform::ComputeMatrixFromComponents()
{
    // Combine: T*R*S
    m_matrix = Translation(m_position);
    m_matrix = m_matrix*RotationEulerAngles(m_rotation);
    m_matrix = m_matrix*Scaling(m_scale);

    m_dirty = false;
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