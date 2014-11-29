#pragma once

#include "Algebra.h"

class Transform
{
public:
    Transform();
    Transform(Matrix4x4& m);

    void        SetMatrix(Matrix4x4& m);
    void        SetPosition(Vector3& position);
    void        SetRotation(Vector3& rotation);
    void        SetScale(Vector3& scale);

    Matrix4x4&  GetMatrix();
    Vector3&    GetPosition();
    Vector3&    GetRotation();
    Vector3&    GetScale();

    bool        IsDirty();
    bool        HasChanged();
    void        ClearChangedFlag();

    void        ComputeMatrixFromComponents();

    static Transform Identity;

    // TODO implement math convenience functions

private:
    Matrix4x4   m_matrix;
    Vector3     m_position;
    Vector3     m_rotation;
    Vector3     m_scale;

    bool        m_dirty;        // specifies whether matrix needs to be recomputed because components have changed
    bool        m_changed;      // specifies whether matrix has changed since last frame
};

Transform operator *(Transform& a, Transform& b);