#pragma once

#include "..\Math\Algebra.h"

class Camera
{
public:
    Vector3 position;
    Vector3 direction;
    Vector3 up;

    Camera() {}
    Camera(Vector3 p, Vector3 d, Vector3 u) : position(p), direction(d), up(u) {}
};