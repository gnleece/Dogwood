#pragma once

#include "Algebra.h"

class Transform
{
public:
    Transform();

    void SetMatrix(Matrix4x4& m);
    void SetPosition(Vector3& position);
    void SetRotation(Vector3& rotation);
    void SetScale(Vector3& scale);

    Matrix4x4&  GetMatrix();
    Vector3&    GetPosition();
    Vector3&    GetRotation();
    Vector3&    GetScale();

    static Transform Identity;

    // TODO implement math convenience functions

private:
    void ComputeMatrixFromComponents();

    Matrix4x4   m_matrix;
    Vector3     m_position;
    Vector3     m_rotation;
    Vector3     m_scale;

    bool        m_dirty;        // specifies whether matrix needs to be recomputed because components have changed
};

Transform operator *(Transform& a, Transform& b);