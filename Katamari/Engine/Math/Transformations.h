#pragma once

#include "Algebra.h"
#include "..\Rendering\Camera.h"

Matrix4x4 Rotation(float angle, eAXIS axis);
Matrix4x4 Translation(const Vector3 & displacement);
Matrix4x4 Scaling(const Vector3 & scale);
Matrix4x4 UniformScaling(float scale);

Matrix4x4 LookAt(const Vector3 & eye, const Vector3 & direction, const Vector3 & up);
Matrix4x4 LookAt(const Camera & camera);
Matrix4x4 PerspectiveProjection(float FOV, float aspect, float near, float far);