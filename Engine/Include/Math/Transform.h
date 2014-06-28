#pragma once

#include "Algebra.h"

// TODO implement stubs

class Transform
{
public:
    void SetMatrix(Matrix4x4& m);
    //void SetPosition(Vector3& position);
    //void SetRotation(Matrix4x4 rotation);
    //void SetScale(Vector3& scale);

    Matrix4x4& GetMatrix();
    //Vector3&    GetPosition();
    //Matrix4x4&  GetRotation();
    //Vector3&    GetScale();

    static Transform Identity;

    // TODO implement math convenience functions

private:
    Matrix4x4 m_matrix;
};

Transform operator *(Transform& a, Transform& b);