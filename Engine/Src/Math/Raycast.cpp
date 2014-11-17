#include "Math\Raycast.h"

// Based on equations from: http://antongerdelan.net/opengl/raycasting.html
bool Raycast::RaycastBoundingSphere(Vector3 rayOrigin,
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
        float sqrtDescr = sqrt(descriminant);
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