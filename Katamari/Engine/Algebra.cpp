#include "Algebra.h"

// TODO (gnleece) make this more robust against floating point weirdness
bool Vector3::Normalize()
{
    float mag = Magnitude();
    if (mag == 0)
        return false;

    m_values[0] /= mag;
    m_values[1] /= mag;
    m_values[2] /= mag;

    return true;
}