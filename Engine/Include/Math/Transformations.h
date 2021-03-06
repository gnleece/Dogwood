#pragma once

#include "Algebra.h"

// TODO rename this file

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

Matrix3x3 InertiaTensorCuboid(const Vector3& halfsizes, float mass);
Matrix3x3 InertiaTensorSphere(float radius, float mass);

void DecomposeTRSMatrix(const Matrix4x4& matrix, Vector3& position, Vector3& rotation, Vector3& scale);
void CalculateTRSMatrix(const Vector3& position, const Vector3& rotation, const Vector3& scale, Matrix4x4& matrix);
void CalculateTRMatrix(const Vector3& position, const Quaternion& rotation, Matrix4x4& matrix);

Quaternion EulerToQuaternion(Vector3& euler);
Vector3 QuaternionToEuler(Quaternion& q);