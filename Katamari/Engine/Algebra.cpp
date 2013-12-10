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

// Return a matrix to represent a counterclockwise rotation of "angle" degrees
// TODO (gnleece) allow rotation about arbitrary axes
Matrix4x4 Matrix4x4::Rotation(float angle, AXIS axis)
{
    Matrix4x4 r;
    angle = DegreesToRadians(angle);

    switch(axis)
    {
        case AXIS_X:
            r = Matrix4x4(  Vector4(1, 0, 0, 0),
                            Vector4(0, cos(angle), -sin(angle), 0),
                            Vector4(0, sin(angle), cos(angle), 0),
                            Vector4(0, 0, 0, 1) );
            break;
        case AXIS_Y:
            r = Matrix4x4(  Vector4(cos(angle), 0, sin(angle), 0),
                            Vector4(0, 1, 0, 0),
                            Vector4(-sin(angle), 0, cos(angle), 0),
                            Vector4(0, 0, 0, 1) );
            break;
        case AXIS_Z:
            r = Matrix4x4(  Vector4(cos(angle), -sin(angle), 0, 0),
                            Vector4(sin(angle), cos(angle), 0, 0),
                            Vector4(0, 0, 1, 0),
                            Vector4(0, 0, 0, 1) );
            break;
        default:
          break;  // return identity matrix
  }
  return r;
}

// Return a matrix to represent a displacement of the given vector.
Matrix4x4 Matrix4x4::Translation(const Vector3& displacement)
{
  Matrix4x4 t( Vector4(1, 0, 0, displacement[0]),
               Vector4(0, 1, 0, displacement[1]),
               Vector4(0, 0, 1, displacement[2]),
               Vector4(0, 0, 0, 1) );
  return t;
}

// Return a matrix to represent a nonuniform scale with the given factors.
Matrix4x4 Matrix4x4::Scaling(const Vector3& scale)
{
  Matrix4x4 s( Vector4(scale[0], 0, 0, 0),
               Vector4(0, scale[1], 0, 0),
               Vector4(0, 0, scale[2], 0),
               Vector4(0, 0, 0, 1) );
  return s;
}

float DegreesToRadians(float degrees)
{
    return degrees*M_PI/180;
}