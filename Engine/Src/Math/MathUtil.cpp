#include "Math/MathUtil.h"

int RandomInt(int min, int max)
{
    return min + (rand() / (RAND_MAX / (max - min)));
}

float RandomFloat(float min, float max)
{
    return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

Vector3 RandomVector(Vector3 min, Vector3 max)
{
    float x = RandomFloat(min.x(), max.x());
    float y = RandomFloat(min.y(), max.y());
    float z = RandomFloat(min.z(), max.z());

    return Vector3(x, y, z);
}