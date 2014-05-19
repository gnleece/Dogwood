#include "Transform.h"

Transform Transform::Identity = Transform();

void Transform::SetMatrix(Matrix4x4& m)
{
    m_matrix = m;
}

Matrix4x4& Transform::GetMatrix()
{
    return m_matrix;
}

Transform operator *(Transform& a, Transform& b)
{
    Transform ret;
    ret.SetMatrix(a.GetMatrix() * b.GetMatrix());
    return ret;
}