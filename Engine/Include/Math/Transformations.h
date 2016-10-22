#pragma once

#include "Algebra.h"

Matrix4x4 Translation(const Vector3& displacement);

Matrix4x4 Rotation(float angle, eAXIS axis);
Matrix4x4 Rotation(float angle, const Vector3& axis);
Matrix4x4 Rotation(const Quaternion& q);
Matrix4x4 RotationEulerAngles(const Vector3& euler);

Matrix4x4 RotationAndTranslation(const Quaternion& q, const Vector3& t);

Matrix4x4 Scaling(const Vector3& scale);
Matrix4x4 UniformScaling(float scale);

Matrix4x4 LookAt(const Vector3& eye, const Vector3& direction, const Vector3& up);
Matrix4x4 PerspectiveProjection(float FOV, float aspect, float near, float far);

void DecomposeMatrix(const Matrix4x4& matrix, Vector3& position, Vector3& rotation, Vector3& scale);
