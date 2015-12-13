#include "Physics/BoundingSphere.h"
#include "Physics/Collider.h"

BoundingSphere::BoundingSphere()
{
    Center = Vector3::Zero;
    Radius = 1;
}

BoundingSphere::BoundingSphere(const Vector3& center, float radius)
{
    Center = center;
    Radius = radius;
}

BoundingSphere::BoundingSphere(const BoundingSphere& a, const BoundingSphere& b)
{
    Vector3 centerOffset = b.Center - a.Center;
    float distanceSqrd = centerOffset.MagnitudeSqrd();
    float radiusDiff = b.Radius - a.Radius;

    // Check whether one sphere completely encloses the other
    if (radiusDiff*radiusDiff >= distanceSqrd)
    {
        if (a.Radius > b.Radius)
        {
            Center = a.Center;
            Radius = a.Radius;
        }
        else
        {
            Center = b.Center;
            Radius = b.Radius;
        }
    }
    // Otherwise, calculate a new bounding sphere that encloses both a and b
    else
    {
        // The diameter of the new sphere is the sum of the diameters of a and b, plus the distance
        // between the points on their edges that lie on the line between their centers.
        // So new radius = 0.5 * new diameter = 0.5 * (2*a.R + 2*b.R + (distance - a.R - b.R)) = 0.5 * (a.R + b.R + distance)
        float distance = sqrt(distanceSqrd);
        Radius = (distance + a.Radius + b.Radius) * 0.5f;

        // The center of the new sphere is based on a's center, then pulled toward b's center
        // by an amount proportional to the spheres' radii.
        Center = a.Center;
        if (distance > 0)
        {
            Center += centerOffset * ((Radius - a.Radius) / distance);
        }
    }
}

BoundingSphere::BoundingSphere(Collider* collider)
{
    if (collider != NULL)
    {
        Center = collider->GetWorldPosition();
        Radius = collider->GetBoundingRadius();
    }
}

bool BoundingSphere::Overlaps(const BoundingSphere* other)
{
    // If the sum of the radii of the two spheres is less than the distance between their centers, they overlap.
    // Compare using squared magnitude of the distance, since this is equivalent and faster to calculate.
    float distanceSquared = (Center - other->Center).MagnitudeSqrd();
    return distanceSquared < (Radius + other->Radius)*(Radius + other->Radius);
}

float BoundingSphere::GetSize()
{
    // Calculate the volume of the sphere
    return (1.33333f) * MATH_PI * Radius * Radius * Radius;
}

float BoundingSphere::GetGrowth(BoundingSphere& volume)
{
    BoundingSphere newSphere(*this, volume);

    // Return a value proportional to the change in surface area
    return newSphere.Radius*newSphere.Radius - Radius*Radius;
}