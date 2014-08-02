#include "Math\Transformations.h"

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

Matrix4x4 RotationEulerAngles(Vector3& euler)
{
    Matrix4x4 r = Rotation(euler[0], AXIS_X);
    r = Rotation(euler[1], AXIS_Y)*r;
    r = Rotation(euler[2], AXIS_Z)*r;
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

// rotation decomposition formula from http://nghiaho.com/?page_id=846
void DecomposeMatrix(Matrix4x4& matrix, Vector3& position, Vector3& rotation, Vector3& scale)
{
    // extract position (4th column)
    position = Vector3(matrix[0][3], matrix[1][3], matrix[2][3]);

    // extract scale
    Vector3 x = Vector3(matrix[0][0], matrix[1][0], matrix[2][0]);
    Vector3 y = Vector3(matrix[0][1], matrix[1][1], matrix[2][1]);
    Vector3 z = Vector3(matrix[0][2], matrix[1][2], matrix[2][2]);
    float scaleX = x.Magnitude();
    float scaleY = y.Magnitude();
    float scaleZ = z.Magnitude();
    scale = Vector3(scaleX, scaleY, scaleZ);

    // extract rotation
    x = x.Normalized();
    y = y.Normalized();
    z = z.Normalized();
    float rotX = RadiansToDegrees(atan2(y[2], z[2]));
    float rotY = RadiansToDegrees(atan2(-x[2], sqrtf(pow(y[2], 2) + pow(z[2], 2))));
    float rotZ = RadiansToDegrees(atan2(x[1], x[0]));
    rotation = Vector3(rotX, rotY, rotZ);
}

// based on pseudocode from 
// http://webglfactory.blogspot.com/2011/06/how-to-create-view-matrix.html
Matrix4x4 LookAt(const Vector3 & eye, const Vector3 & direction, const Vector3 & up)
{
    Vector3 v_z = normalize(-1*direction);
    Vector3 v_x = normalize(cross(up, v_z));
    Vector3 v_y = cross(v_z, v_x);
    Vector3 w = Vector3(dot(v_x, eye), dot(v_y, eye), dot(v_z, eye));

    Matrix4x4 m = Matrix4x4(Vector4(v_x[0], v_y[0], v_z[0], w[0]),
                            Vector4(v_x[1], v_y[1], v_z[1], w[1]),
                            Vector4(v_x[2], v_y[2], v_z[2], w[2]),
                            Vector4(0, 0, 0, 1));

    return m;
}

Matrix4x4 LookAt(const Camera & camera)
{
    return LookAt(camera.position, camera.direction, camera.up);
}

Matrix4x4 PerspectiveProjection(float FOV, float aspect, float near, float far)
{
    float f = 1/tan(DegreesToRadians(FOV/2));
    return Matrix4x4(Vector4(f/aspect,0,0,0),
                     Vector4(0,f,0,0),
                     Vector4(0,0,(far+near)/(near-far),2*far*near/(near-far)),
                     Vector4(0,0,-1,0));
}