#pragma once

#include "Algebra.h"

class Raycast
{
public: 
    struct HitInfo
    {
        Vector3 point;
        float distance;

        HitInfo() {};
        HitInfo(Vector3 p, float d) { point = p, distance = d; }
    };

    static bool RaycastBoundingSphere(Vector3 rayOrigin,
                                      Vector3 rayDirection,
                                      float sphereRadius,
                                      Vector3 spherePosition,
                                      HitInfo& hitInfo);
};