#pragma once

#include "Algebra.h"

class GameObjectBase;
class MeshInstance;

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

    static bool RaycastSphere(Vector3 rayOrigin,
                              Vector3 rayDirection,
                              float sphereRadius,
                              Vector3 spherePosition,
                              HitInfo& hitInfo);

    static bool RaycastTriangle(Vector3 rayOrigin,
                                Vector3 rayDirection,
                                Vector3* triangle,
                                HitInfo& hitInfo);

    static bool RaycastMesh(Vector3 rayOrigin,
                            Vector3 rayDirection,
                            MeshInstance* meshInstance,
                            HitInfo& hitInfo);

    static bool RaycastGameObject(Vector3 rayOrigin,
                                  Vector3 rayDirection,
                                  GameObjectBase* gameObject,
                                  bool recursive,
                                  GameObjectBase*& hitObject,
                                  HitInfo& hitInfo);
};