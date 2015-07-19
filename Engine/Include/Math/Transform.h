#pragma once

#include "Algebra.h"

enum TransformVectorType { eVector_Position, eVector_Rotation, eVector_Scale };

class Transform
{
public:
    Transform();
    Transform(Matrix4x4& m);

    void        SetMatrix(Matrix4x4& m);
    void        SetPosition(Vector3& position);
    void        SetRotation(Vector3& rotation);
    void        SetScale(Vector3& scale);
    void        SetVector(Vector3& vector, TransformVectorType type);

    Matrix4x4&  GetMatrix();
    Vector3&    GetPosition();
    Vector3&    GetRotation();
    Vector3&    GetScale();
    Vector3&    GetVector(TransformVectorType type);

    Vector3&    GetRight();
    Vector3&    GetUp();
    Vector3&    GetForward();

    bool        IsDirty();
    bool        HasChanged();
    void        ClearChangedFlag();

    void        ComputeMatrixFromComponents();

    static Transform Identity;

    // TODO implement math convenience functions

private:
    void        CalculateDirectionVectors();

    Matrix4x4   m_matrix;

    Vector3     m_position;
    Vector3     m_rotation;
    Vector3     m_scale;

    Vector3     m_right;
    Vector3     m_up;
    Vector3     m_forward;

    bool        m_dirty;        // specifies whether matrix needs to be recomputed because components have changed
    bool        m_changed;      // specifies whether matrix has changed since last frame
};

Transform operator *(Transform& a, Transform& b);
Vector4 operator *(Transform& m, Vector4& v);