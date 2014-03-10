#include "Transformations.h"

// Return a matrix to represent a counterclockwise rotation of "angle" degrees
// TODO (gnleece) allow rotation about arbitrary axes
Matrix4x4 Rotation(float angle, eAXIS axis)
{
    Matrix4x4 r;
    angle = DegreesToRadians(angle);

    switch(axis)
    {
        case AXIS_X:
            r = Matrix4x4(Vector4(1, 0, 0, 0),
                          Vector4(0, cos(angle), -sin(angle), 0),
                          Vector4(0, sin(angle), cos(angle), 0),
                          Vector4(0, 0, 0, 1));
            break;
        case AXIS_Y:
            r = Matrix4x4(Vector4(cos(angle), 0, sin(angle), 0),
                          Vector4(0, 1, 0, 0),
                          Vector4(-sin(angle), 0, cos(angle), 0),
                          Vector4(0, 0, 0, 1));
            break;
        case AXIS_Z:
            r = Matrix4x4(Vector4(cos(angle), -sin(angle), 0, 0),
                          Vector4(sin(angle), cos(angle), 0, 0),
                          Vector4(0, 0, 1, 0),
                          Vector4(0, 0, 0, 1));
            break;
        default:
          break;  // return identity matrix
  }
  return r;
}

// Return a matrix to represent a displacement of the given vector.
Matrix4x4 Translation(const Vector3& displacement)
{
  Matrix4x4 t(Vector4(1, 0, 0, displacement[0]),
              Vector4(0, 1, 0, displacement[1]),
              Vector4(0, 0, 1, displacement[2]),
              Vector4(0, 0, 0, 1));
  return t;
}

// Return a matrix to represent a nonuniform scale with the given factors.
Matrix4x4 Scaling(const Vector3& scale)
{
  Matrix4x4 s(Vector4(scale[0], 0, 0, 0),
              Vector4(0, scale[1], 0, 0),
              Vector4(0, 0, scale[2], 0),
              Vector4(0, 0, 0, 1));
  return s;
}

Matrix4x4 UniformScaling(float scale)
{
    return Scaling(Vector3(scale, scale, scale));
}

// based on pseudocode from 
// http://webglfactory.blogspot.com/2011/06/how-to-create-view-matrix.html
Matrix4x4 LookAt(Vector3 eye, Vector3 target, Vector3 up)
{
    Vector3 v_z = normalize(eye - target);
    Vector3 v_x = normalize(cross(up, v_z));
    Vector3 v_y = cross(v_z, v_x);
    Vector3 w = Vector3(dot(v_x, eye), dot(v_y, eye), dot(v_z, eye));

    Matrix4x4 m = Matrix4x4(Vector4(v_x[0], v_y[0], v_z[0], w[0]),
                            Vector4(v_x[1], v_y[1], v_z[1], w[1]),
                            Vector4(v_x[2], v_y[2], v_z[2], w[2]),
                            Vector4(0, 0, 0, 1));

    return m;
}

Matrix4x4 PerspectiveProjection(float FOV, float aspect, float near, float far)
{
    float f = 1/tan(DegreesToRadians(FOV/2));
    return Matrix4x4(Vector4(f/aspect,0,0,0),
                     Vector4(0,f,0,0),
                     Vector4(0,0,(far+near)/(near-far),2*far*near/(near-far)),
                     Vector4(0,0,-1,0));
}