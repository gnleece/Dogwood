#pragma once

#include "Algebra.h"

Matrix4x4 Rotation(float angle, AXIS axis);
Matrix4x4 Translation(const Vector3& displacement);
Matrix4x4 Scaling(const Vector3& scale);

Matrix4x4 LookAt(Vector3 eye, Vector3 target, Vector3 up);
Matrix4x4 PerspectiveProjection(float FOV, float aspect, float near, float far);