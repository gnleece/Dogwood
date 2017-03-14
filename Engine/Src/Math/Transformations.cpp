#include "Math\Transformations.h"

#include <math.h>

// Return a matrix to represent a displacement of the given vector.
Matrix4x4 Translation(const Vector3& displacement)
{
    Matrix4x4 t(Vector4(1, 0, 0, displacement[0]),
        Vector4(0, 1, 0, displacement[1]),
        Vector4(0, 0, 1, displacement[2]),
        Vector4(0, 0, 0, 1));
    return t;
}

// Return a matrix to represent a counterclockwise rotation of "angle" degrees (in radians)
Matrix4x4 Rotation(float angle, eAXIS axis)
{
    Matrix4x4 r;
    angle = DegreesToRadians(angle);

    switch (axis)
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

// Return a matrix to represent a counterclockwise rotation of "angle" degrees (in radians)
// Math from http://paulbourke.net/geometry/rotate/
Matrix4x4 Rotation(float angle, const Vector3& axis)
{
    Matrix4x4 r;

    Vector3 normalizedAxis = axis.Normalized();
    float a = normalizedAxis.x();
    float b = normalizedAxis.y();
    float c = normalizedAxis.z();
    float d = sqrtf(b*b + c*c);

    Matrix4x4 Rx, Rx_i;

    if (d > 0)
    {
        Rx = Matrix4x4(Vector4(1, 0, 0, 0),
                       Vector4(0, c/d, -b/d, 0),
                       Vector4(0, b/d, c/d, 0),
                       Vector4(0, 0, 0, 1));

        Rx_i = Matrix4x4(Vector4(1, 0, 0, 0),
                         Vector4(0, c/d, b/d, 0),
                         Vector4(0, -b/d, c/d, 0),
                         Vector4(0, 0, 0, 1));
    }

    Matrix4x4 Ry = Matrix4x4(Vector4(d, 0, -a, 0),
                             Vector4(0, 1, 0, 0),
                             Vector4(a, 0, d, 0),
                             Vector4(0, 0, 0, 1));

    Matrix4x4 Ry_i = Matrix4x4(Vector4(d, 0, a, 0),
                               Vector4(0, 1, 0, 0),
                               Vector4(-a, 0, d, 0),
                               Vector4(0, 0, 0, 1));

    Matrix4x4 Rz = Rotation(angle, AXIS_Z);

    r = Rx_i * Ry_i * Rz * Ry * Rx;

    return r;
}

Matrix4x4 Rotation(const Quaternion& q)
{
    Matrix4x4 r;

    r[0][0] = 1 - (2 * q.j()*q.j() + 2 * q.k()*q.k());
    r[0][1] = 2 * q.i()*q.j() + 2 * q.k()*q.r();
    r[0][2] = 2 * q.i()*q.k() - 2 * q.j()*q.r();
    r[1][0] = 2 * q.i()*q.j() - 2 * q.k()*q.r();
    r[1][1] = 1 - (2 * q.i()*q.i() + 2 * q.k()*q.k());
    r[1][2] = 2 * q.j()*q.k() + 2 * q.i()*q.r();
    r[2][0] = 2 * q.i()*q.k() + 2 * q.j()*q.r();
    r[2][1] = 2 * q.j()*q.k() - 2 * q.i()*q.r();
    r[2][2] = 1 - (2 * q.i()*q.i() + 2 * q.j()*q.j());

    return r;
}

Matrix4x4 RotationEulerAngles(const Vector3& euler)
{
    Matrix4x4 r = Rotation(euler[0], AXIS_X);
    r = Rotation(euler[1], AXIS_Y)*r;
    r = Rotation(euler[2], AXIS_Z)*r;
    return r;
}

Matrix4x4 RotationAndTranslation(const Quaternion& q, const Vector3& t)
{
    Matrix4x4 r = Rotation(q);
    r[0][3] = t[0];
    r[1][3] = t[1];
    r[2][3] = t[2];
    return r;
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

// based on pseudocode from http://www.3dgep.com/understanding-the-view-matrix/
Matrix4x4 LookAt(const Vector3 & eye, const Vector3 & direction, const Vector3 & up)
{
    Vector3 v_z = normalize(direction);
    Vector3 v_x = normalize(cross(up, v_z));
    Vector3 v_y = cross(v_z, v_x);
    Vector3 w = Vector3(-dot(v_x, 1*eye), -dot(v_y, 1*eye), -dot(v_z, 1*eye));

    Matrix4x4 m = Matrix4x4(Vector4(v_x[0], v_y[0], v_z[0], 0),
                            Vector4(v_x[1], v_y[1], v_z[1], 0),
                            Vector4(v_x[2], v_y[2], v_z[2], 0),
                            Vector4(w[0],   w[1],   w[2],   1));

    return m.Transpose();
}

Matrix4x4 PerspectiveProjection(float FOV, float aspect, float near, float far)
{
    float f = 1/tan(DegreesToRadians(FOV/2));
    return Matrix4x4(Vector4(f/aspect,0,0,0),
                     Vector4(0,f,0,0),
                     Vector4(0,0,(far+near)/(near-far),2*far*near/(near-far)),
                     Vector4(0,0,-1,0));
}

Matrix3x3 InertiaTensorCuboid(const Vector3& halfsizes, float mass)
{
    Matrix3x3 m;
    Vector3 squares = halfsizes.ComponentwiseProduct(halfsizes);
    float s = 0.08333f * mass;      // 1/12 = 0.08333...
    m.SetDiagonal(Vector3(s*(squares.y() + squares.z()),
                          s*(squares.x() + squares.z()),
                          s*(squares.x() + squares.y())));
    return m;
}

Matrix3x3 InertiaTensorSphere(float radius, float mass)
{
    Matrix3x3 m;
    float d = 0.4f*mass*radius*radius;       // 2/5 = 0.4
    m.SetDiagonal(Vector3(d, d, d));
    return m;
}

// rotation decomposition formula from http://nghiaho.com/?page_id=846
void DecomposeTRSMatrix(const Matrix4x4& matrix, Vector3& position, Vector3& rotation, Vector3& scale)
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

void CalculateTRSMatrix(const Vector3& position, const Vector3& rotation, const Vector3& scale, Matrix4x4& matrix)
{
    matrix = Translation(position);
    matrix = matrix*RotationEulerAngles(rotation);
    matrix = matrix*Scaling(scale);
}

// From https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
Quaternion EulerToQuaternion(Vector3& euler)
{
    // TODO needs testing

    Quaternion q;

    float t0 = std::cos(euler.y() * 0.5f);
    float t1 = std::sin(euler.y() * 0.5f);
    float t2 = std::cos(euler.z() * 0.5f);
    float t3 = std::sin(euler.z() * 0.5f);
    float t4 = std::cos(euler.x() * 0.5f);
    float t5 = std::sin(euler.x() * 0.5f);

    // TODO w x y z -> i j k r?
    q.SetI(t0 * t2 * t4 + t1 * t3 * t5);
    q.SetJ(t0 * t3 * t4 - t1 * t2 * t5);
    q.SetK(t0 * t2 * t5 + t1 * t3 * t4);
    q.SetR(t1 * t2 * t4 - t0 * t3 * t5);

    return q;
}

// From https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
Vector3 QuaternionToEuler(Quaternion& q)
{
    // TODO needs testing

    Vector3 euler;

    float k_sqr = q.k() * q.k();

    float t0 = 2 * (q.i() * q.j() + q.k() * q.r());
    float t1 = 1 - 2 * (q.j() * q.j() + k_sqr);
    euler.SetZ(std::atan2(t0, t1));

    float t2 = 2 * (q.i() * q.k() - q.r() * q.j());
    Clamp(t2, -1.0f, 1.0f);
    euler.SetX(std::asin(t2));

    float t3 = 2 * (q.i() * q.r() + q.j() * q.k());
    float t4 = 1 - 2 * (k_sqr + q.r() * q.r());
    euler.SetY(std::atan2(t3, t4));

    return euler;
}