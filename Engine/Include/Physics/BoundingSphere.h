#pragma once

#include "Math/Algebra.h"

namespace DgwdPhysics
{
    struct BoundingSphere
    {
        BoundingSphere(const Vector3& center, float radius);
        BoundingSphere(const BoundingSphere& a, const BoundingSphere& b);

        bool        Overlaps(const BoundingSphere* other);
        float       GetSize();
        float       GetGrowth(BoundingSphere& volume);

        Vector3     Center;
        float       Radius;
    };
}