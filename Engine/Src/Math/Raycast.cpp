#include "Math\Raycast.h"

#include "GameObjectBase.h"
#include "Rendering/MeshInstance.h"

// Based on equations from: http://antongerdelan.net/opengl/raycasting.html
bool Raycast::RaycastSphere(Vector3 rayOrigin,
    Vector3 rayDirection,
    float sphereRadius,
    Vector3 spherePosition,
    HitInfo& hitInfo)
{
    // Calculate descriminant of intersection equation
    Vector3 objectToRayOrigin = rayOrigin - spherePosition;
    float b = rayDirection.Dot(objectToRayOrigin);
    float c = objectToRayOrigin.Dot(objectToRayOrigin) - sphereRadius*sphereRadius;
    float descriminant = b*b - c;

    // Negative descriminant = no solution = no intersection
    if (descriminant < 0)
    {
        return false;
    }
    // Otherwise there is at least one solution
    else
    {
        // Calculate solution(s) (if descriminant is 0, they are equal)
        float sqrtDescr = sqrtf(descriminant);
        float t_1 = -b + sqrtDescr;
        float t_2 = -b - sqrtDescr;

        // Take the solution with the smallest distance
        float distance = std::fminf(t_1, t_2);

        // Calculate hit position
        Vector3 point = rayOrigin + distance*rayDirection.Normalized();

        hitInfo = HitInfo(point, distance);
        return true;
    }
}

// Based on http://www.lighthouse3d.com/tutorials/maths/ray-triangle-intersection/
bool Raycast::RaycastTriangle(Vector3 rayOrigin,
    Vector3 rayDirection,
    Vector3* triangle,
    HitInfo& hitInfo)
{
    Vector3 e1, e2, h, s, q;
    float a, f, u, v, t;

    Vector3 v0 = triangle[0];
    Vector3 v1 = triangle[1];
    Vector3 v2 = triangle[2];

    e1 = v1 - v0;
    e2 = v2 - v0;

    h = rayDirection.Cross(e2);
    a = e1.Dot(h);

    if (Approximately(a, 0))
        return false;

    f = 1 / a;
    s = rayOrigin - v0;
    u = f * s.Dot(h);

    if (u < 0 || u > 1)
        return false;

    q = s.Cross(e1);
    v = f * rayDirection.Dot(q);

    if (v < 0 || u + v > 1)
        return false;

    t = f * e2.Dot(q);

    if (t > 0 && !Approximately(t, 0))
    {
        hitInfo.point = rayOrigin + t * rayDirection;
        hitInfo.distance = (hitInfo.point - rayOrigin).Magnitude();
        return true;
    }

    return false;
}

bool Raycast::RaycastMesh(Vector3 rayOrigin,
    Vector3 rayDirection,
    MeshInstance* meshInstance,
    HitInfo& hitInfo)
{
    // Iterate through each triangle in the mesh
    Vector3 triangle[3];
    bool hit = false;
    float closestHitDistance = FLT_MAX;
    for (int i = 0; i < meshInstance->GetTriangleCount(); i++)
    {
        meshInstance->GetTriangle(i, triangle, true);

        // Raycast against the current triangle
        HitInfo currentHitInfo;
        if (RaycastTriangle(rayOrigin, rayDirection, triangle, currentHitInfo))
        {
            hit = true;
            if (currentHitInfo.distance < closestHitDistance)
            {
                closestHitDistance = currentHitInfo.distance;
                hitInfo = currentHitInfo;
            }
        }
    }

    return hit;
}

bool Raycast::RaycastGameObject(Vector3 rayOrigin,
    Vector3 rayDirection,
    GameObjectBase* gameObject,
    bool recursive,
    GameObjectBase*& hitObject,
    HitInfo& hitInfo)
{
    bool hit = false;
    HitInfo bestHitInfo;
    bestHitInfo.distance = FLT_MAX;
    GameObjectBase* bestGameObject = NULL;

    // Raycast against the given game object, if it has a mesh
    if (gameObject->GetMeshInstance() != NULL && gameObject->GetMeshInstance()->GetMesh() != NULL)
    {
        // Calculate the radius for a bounding sphere for the mesh
        float radius = gameObject->GetMeshInstance()->GetBoundingRadius(true);
        Vector3 objectCenter = gameObject->GetTransform().GetWorldPosition();

        // Do the raycast
        HitInfo currentHitInfo;
        hit = Raycast::RaycastSphere(rayOrigin, rayDirection, radius, objectCenter, currentHitInfo);
        if (hit)
        {
            // We hit the bounding sphere, so do a more precise raycast against the mesh itself
            hit = Raycast::RaycastMesh(rayOrigin, rayDirection, gameObject->GetMeshInstance(), currentHitInfo);
            if (hit)
            {
                bestHitInfo = currentHitInfo;
                bestGameObject = gameObject;
            }
        }
    }

    if (recursive)
    {
        // Raycast against children
        std::vector<GameObjectBase*>::iterator childIter;
        vector<GameObjectBase*> children = gameObject->GetChildren();
        for (childIter = children.begin(); childIter != children.end(); childIter++)
        {
            HitInfo currentHitInfo;
            GameObjectBase* currentHitObject;
            bool hitChild = RaycastGameObject(rayOrigin, rayDirection, *childIter, true, currentHitObject, currentHitInfo);
            if (hitChild)
            {
                hit = true;
                if (currentHitInfo.distance < bestHitInfo.distance)
                {
                    bestHitInfo = currentHitInfo;
                    bestGameObject = currentHitObject;
                }
            }
        }
    }

    // Return object with smallest distance from ray origin
    if (hit)
    {
        hitInfo = bestHitInfo;
        hitObject = bestGameObject;
        return true;
    }

    return false;
}