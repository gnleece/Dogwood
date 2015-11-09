#pragma once

#include "Algebra.h"
#include <vector>

using std::vector;

enum TransformVectorType { eVector_Position, eVector_Rotation, eVector_Scale };

class Transform
{
public:
    static      Transform Identity;

    Transform();
    Transform(Matrix4x4& m);
    ~Transform();

    void        SetParent(Transform* parent);

    void        SetLocalMatrix(Matrix4x4& m);
    void        SetLocalPosition(Vector3& position);
    void        SetLocalRotation(Vector3& rotation);
    void        SetLocalScale(Vector3& scale);
    void        SetLocalVector(Vector3& vector, TransformVectorType type);

    Matrix4x4&  GetLocalMatrix();
    Vector3&    GetLocalPosition();
    Vector3&    GetLocalRotation();
    Vector3&    GetLocalScale();
    Vector3&    GetLocalVector(TransformVectorType type);

    Matrix4x4&  GetWorldMatrix();
    Vector3&    GetWorldPosition();
    Vector3&    GetWorldRotation();
    Vector3&    GetWorldScale();
    Vector3&    GetWorldVector(TransformVectorType type);

    Vector3&    GetRight();
    Vector3&    GetUp();
    Vector3&    GetForward();

    // TODO implement math convenience functions

private:
    void        RemoveChild(Transform* transform);

    void        SetRecomputeWorldFlag();
    void        ClearRecomputeWorldFlag();
    void        RecomputeLocalIfDirty();
    void        RecomputeWorldIfDirty();

    void        CalculateDirectionVectors();

    Transform*          m_parent;
    vector<Transform*>  m_children;

    Matrix4x4           m_localMatrix;
    Vector3             m_localPosition;
    Vector3             m_localRotation;
    Vector3             m_localScale;

    Matrix4x4           m_worldMatrix;
    Vector3             m_worldPosition;
    Vector3             m_worldRotation;
    Vector3             m_worldScale;

    Vector3             m_right;
    Vector3             m_up;
    Vector3             m_forward;

    bool                m_recomputeLocal;
    bool                m_recomputeWorld;
};

Transform operator *(Transform& a, Transform& b);
Vector4 operator *(Transform& m, Vector4& v);