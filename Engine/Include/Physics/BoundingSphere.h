#pragma once

#include "Math/Algebra.h"

class Collider;

struct BoundingSphere
{
    BoundingSphere();
    BoundingSphere(const Vector3& center, float radius);
    BoundingSphere(const BoundingSphere& a, const BoundingSphere& b);
    BoundingSphere(Collider* collider);

    bool        Overlaps(const BoundingSphere* other);
    float       GetSize();
    float       GetGrowth(BoundingSphere& volume);

    Vector3     Center;
    float       Radius;
};