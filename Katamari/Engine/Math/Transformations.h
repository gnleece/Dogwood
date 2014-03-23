#pragma once

#include "Algebra.h"

class Camera
{
public:
    Vector3 position;
    Vector3 direction;
    Vector3 up;

    Camera() {}
    Camera(Vector3 p, Vector3 d, Vector3 u) : position(p), direction(d), up(u) {}
};

Matrix4x4 Rotation(float angle, eAXIS axis);
Matrix4x4 Translation(const Vector3 & displacement);
Matrix4x4 Scaling(const Vector3 & scale);
Matrix4x4 UniformScaling(float scale);

Matrix4x4 LookAt(const Vector3 & eye, const Vector3 & direction, const Vector3 & up);
Matrix4x4 LookAt(const Camera & camera);
Matrix4x4 PerspectiveProjection(float FOV, float aspect, float near, float far);