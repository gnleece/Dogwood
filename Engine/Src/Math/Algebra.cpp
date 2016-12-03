#include "Math\Algebra.h"
#include <algorithm>
#include <limits>

Vector2::Vector2()
{
    m_values[0] = 0.0f;
    m_values[1] = 0.0f;
}

Vector2::Vector2(const Vector2& other)
{
    m_values[0] = other.m_values[0];
    m_values[1] = other.m_values[1];
}

Vector2::Vector2(float x, float y)
{
    m_values[0] = x;
    m_values[1] = y;
}

Vector2& Vector2::operator =(const Vector2 other)
{
    m_values[0] = other.m_values[0];
    m_values[1] = other.m_values[1];
    return *this;
}

bool Vector2::operator ==(const Vector2 &other) const
{
    return (m_values[0] == other.m_values[0] &&
            m_values[1] == other.m_values[1]);
}

float& Vector2::operator[](int i)
{
    return m_values[i];
}

float Vector2::operator[](int i) const
{
    return m_values[i];
}

float* Vector2::Start()
{
    return m_values;
}

float Vector2::x() const
{
    return m_values[0];
}

float Vector2::y() const
{
    return m_values[1];
}

void Vector2::SetX(float x)
{
    m_values[0] = x;
}

void Vector2::SetY(float y)
{
    m_values[1] = y;
}

Vector2 operator +(const Vector2& a, const Vector2& b)
{
    return Vector2(a[0] + b[0], a[1] + b[1]);
}

Vector2 operator -(const Vector2& a, const Vector2& b)
{
    return Vector2(a[0] - b[0], a[1] - b[1]);
}

Vector2 Vector2::Zero = Vector2(0.f, 0.f);
Vector2 Vector2::One = Vector2(1.f, 1.f);

Vector3::Vector3()
{
    m_values[0] = 0.0f;
    m_values[1] = 0.0f;
    m_values[2] = 0.0f;
}

Vector3::Vector3(const Vector3& other)
{
    m_values[0] = other.m_values[0];
    m_values[1] = other.m_values[1];
    m_values[2] = other.m_values[2];
}

Vector3::Vector3(float x, float y, float z)
{
    m_values[0] = x;
    m_values[1] = y;
    m_values[2] = z;
}

Vector3& Vector3::operator =(const Vector3& other)
{
    m_values[0] = other.m_values[0];
    m_values[1] = other.m_values[1];
    m_values[2] = other.m_values[2];
    return *this;
}

Vector3& Vector3::operator +=(const Vector3& other)
{
    m_values[0] = m_values[0] + other.m_values[0];
    m_values[1] = m_values[1] + other.m_values[1];
    m_values[2] = m_values[2] + other.m_values[2];
    return *this;
}

Vector3& Vector3::operator -=(const Vector3& other)
{
    m_values[0] = m_values[0] - other.m_values[0];
    m_values[1] = m_values[1] - other.m_values[1];
    m_values[2] = m_values[2] - other.m_values[2];
    return *this;
}

Vector3& Vector3::operator *=(float s)
{
    m_values[0] *= s;
    m_values[1] *= s;
    m_values[2] *= s;
    return *this;
}

bool Vector3::operator ==(const Vector3 &other) const
{
    return (m_values[0] == other.m_values[0] &&
            m_values[1] == other.m_values[1] &&
            m_values[2] == other.m_values[2]);
}

float& Vector3::operator[](int i)
{
    return m_values[i];
}

float Vector3::operator[](int i) const
{
    return m_values[i];
}

float* Vector3::Start()
{
    return m_values;
}

float Vector3::x() const
{
    return m_values[0];
}

float Vector3::y() const
{
    return m_values[1];
}

float Vector3::z() const
{
    return m_values[2];
}

void Vector3::SetX(float x)
{
    m_values[0] = x;
}

void Vector3::SetY(float y)
{
    m_values[1] = y;
}

void Vector3::SetZ(float z)
{
    m_values[2] = z;
}

float Vector3::MagnitudeSqrd() const
{
    return m_values[0]*m_values[0] +
           m_values[1]*m_values[1] +
           m_values[2]*m_values[2];
}

float Vector3::Magnitude() const
{
    return sqrtf(MagnitudeSqrd());
}

float Vector3::Dot(const Vector3& other) const
{
    return m_values[0]*other.m_values[0] +
           m_values[1]*other.m_values[1] +
           m_values[2]*other.m_values[2];
}

Vector3 Vector3::Cross(const Vector3& other) const
{
    return Vector3(m_values[1]*other.m_values[2] - m_values[2]*other.m_values[1],
                   m_values[2]*other.m_values[0] - m_values[0]*other.m_values[2],
                   m_values[0]*other.m_values[1] - m_values[1]*other.m_values[0]);
}

// TODO make this more robust against floating point weirdness
Vector3 Vector3::Normalized() const
{
    Vector3 ret = *this;
    float mag = ret.Magnitude();
    if (mag == 0)
        return ret;

    ret[0] /= mag;
    ret[1] /= mag;
    ret[2] /= mag;

    return ret;
}

void Vector3::Normalize()
{
    Vector3 n = Normalized();
    m_values[0] = n[0];
    m_values[1] = n[1];
    m_values[2] = n[2];
}

float Vector3::MaxElement()
{
    return fmax(m_values[0], fmax(m_values[1], m_values[2]));
}

float Vector3::MinElement()
{
    return fmin(m_values[0], fmin(m_values[1], m_values[2]));
}

Vector3 Vector3::ComponentwiseInverse() const
{
    float x = m_values[0] == 0 ? 0 : 1 / m_values[0];
    float y = m_values[1] == 0 ? 0 : 1 / m_values[1];
    float z = m_values[2] == 0 ? 0 : 1 / m_values[2];
    return Vector3(x, y, z);
}

bool Vector3::HasZeroComponent() const
{
    return (m_values[0] == 0 || m_values[1] == 0 || m_values[2] == 0);
}

void Vector3::DebugPrint()
{
    printf("%f\t%f\t%f\n", m_values[0], m_values[1], m_values[2]);
}

Vector3 operator +(const Vector3& a, const Vector3& b)
{
  return Vector3(a[0]+b[0], a[1]+b[1], a[2]+b[2]);
}

Vector3 operator -(const Vector3& a, const Vector3& b)
{
  return Vector3(a[0]-b[0], a[1]-b[1], a[2]-b[2]);
}

Vector3 operator *(float s, const Vector3& v)
{
  return Vector3(s*v[0], s*v[1], s*v[2]);
}

Vector3 operator *(const Vector3& v, float s)
{
    return Vector3(s*v[0], s*v[1], s*v[2]);
}

Vector3 cross(const Vector3& a, const Vector3& b)
{
  return a.Cross(b);
}

float dot(const Vector3& a, const Vector3& b)
{
    return a.Dot(b);
}

Vector3 normalize(Vector3 v)
{
    return v.Normalized();
}

float Vector3::Distance(Vector3& a, Vector3& b)
{
    return (a - b).Magnitude();
}

Vector3 Vector3::Zero       = Vector3(0.f, 0.f, 0.f);
Vector3 Vector3::One        = Vector3(1.f, 1.f, 1.f);
Vector3 Vector3::Right      = Vector3(1.f, 0.f, 0.f);
Vector3 Vector3::Up         = Vector3(0.f, 1.f, 0.f);
Vector3 Vector3::Forward    = Vector3(0.f, 0.f, 1.f);

Vector4::Vector4()
{
    m_values[0] = 0.0f;
    m_values[1] = 0.0f;
    m_values[2] = 0.0f;
    m_values[3] = 0.0f;
}

Vector4::Vector4(const Vector4& other)
{
    m_values[0] = other.m_values[0];
    m_values[1] = other.m_values[1];
    m_values[2] = other.m_values[2];
    m_values[3] = other.m_values[4];
}

Vector4::Vector4(float x, float y, float z, float w)
{
    m_values[0] = x;
    m_values[1] = y;
    m_values[2] = z;
    m_values[3] = w;
}

Vector4::Vector4(Vector3& v, float w)
{
    m_values[0] = v[0];
    m_values[1] = v[1];
    m_values[2] = v[2];
    m_values[3] = w;
}

Vector4& Vector4::operator =(const Vector4& other)
{
    m_values[0] = other.m_values[0];
    m_values[1] = other.m_values[1];
    m_values[2] = other.m_values[2];
    m_values[3] = other.m_values[3];
    return *this;
}

float& Vector4::operator[](int i)
{
    return m_values[i];
}
float Vector4::operator[](int i) const
{
    return m_values[i];
}

float Vector4::x() const
{
    return m_values[0];
}

float Vector4::y() const
{
    return m_values[1];
}

float Vector4::z() const
{
    return m_values[2];
}

float Vector4::w() const
{
    return m_values[3];
}

void Vector4::SetX(float x)
{
    m_values[0] = x;
}

void Vector4::SetY(float y)
{
    m_values[1] = y;
}

void Vector4::SetZ(float z)
{
    m_values[2] = z;
}

void Vector4::SetW(float w)
{
    m_values[3] = w;
}

float Vector4::MagnitudeSqrd() const
{
    return m_values[0] * m_values[0] +
           m_values[1] * m_values[1] +
           m_values[2] * m_values[2] +
           m_values[3] * m_values[3];
}

float Vector4::Magnitude() const
{
    return sqrtf(MagnitudeSqrd());
}

// TODO make this more robust against floating point weirdness
Vector4 Vector4::Normalized() const
{
    Vector4 ret = *this;
    float mag = ret.Magnitude();
    if (mag == 0)
        return ret;

    ret[0] /= mag;
    ret[1] /= mag;
    ret[2] /= mag;
    ret[3] /= mag;

    return ret;
}

void Vector4::Normalize()
{
    Vector4 n = Normalized();
    m_values[0] = n[0];
    m_values[1] = n[1];
    m_values[2] = n[2];
    m_values[3] = n[3];
}

float Vector4::MaxElement()
{
    return fmax(m_values[0], fmax(m_values[1], fmax(m_values[2], m_values[3])));
}

float Vector4::MinElement()
{
    return fmin(m_values[0], fmin(m_values[1], fmin(m_values[2], m_values[3])));
}

Vector4 operator +(const Vector4& a, const Vector4& b)
{
  return Vector4(a[0]+b[0], a[1]+b[1], a[2]+b[2], a[3]+b[3]);
}

Vector4 operator -(const Vector4& a, const Vector4& b)
{
  return Vector4(a[0]-b[0], a[1]-b[1], a[2]-b[2], a[3]-b[3]);
}

Vector4 operator *(float s, const Vector4& v)
{
  return Vector4(s*v[0], s*v[1], s*v[2], s*v[3]);
}

Vector3 Vector4::xyz()
{
    return Vector3(m_values[0], m_values[1], m_values[2]);
}

void Vector4::DebugPrint()
{
    printf("%f\t%f\t%f\t%f\n", m_values[0], m_values[1], m_values[2], m_values[3]);
}


Matrix3x3::Matrix3x3()
{
    // default is identity matrix
    std::fill(m_values, m_values + m_size, 0.0f);
    m_values[0] = 1.0f;
    m_values[4] = 1.0f;
    m_values[8] = 1.0f;
}

Matrix3x3::Matrix3x3(const Matrix3x3& other)
{
    std::copy(other.m_values, other.m_values + m_size, m_values);
}

Matrix3x3::Matrix3x3(Vector3 row0, Vector3 row1, Vector3 row2)
{
    m_values[0] = row0[0];
    m_values[1] = row0[1];
    m_values[2] = row0[2];

    m_values[3] = row1[0];
    m_values[4] = row1[1];
    m_values[5] = row1[2];

    m_values[6] = row2[0];
    m_values[7] = row2[1];
    m_values[8] = row2[2];
}

Matrix3x3& Matrix3x3::operator =(const Matrix3x3& other)
{
    std::copy(other.m_values, other.m_values + m_size, m_values);
    return *this;
}

Vector3 Matrix3x3::operator[](int row) const
{
    return Row(row);
}
float* Matrix3x3::operator[](int row)
{
    return Row(row);
}

const float* Matrix3x3::Start() const
{
    return (float*)m_values;
}
const float* Matrix3x3::End() const
{
    return (float*)(m_values + m_size);
}

float* Matrix3x3::Row(int row)
{
    return (float*)m_values + 3 * row;
}

Vector3 Matrix3x3::Row(int row) const
{
    return Vector3(m_values[3 * row], m_values[3 * row + 1], m_values[3 * row + 2]);
}

Vector3 Matrix3x3::Column(int col) const
{
    return Vector3(m_values[col], m_values[3 + col], m_values[6 + col]);
}

void Matrix3x3::SetColumns(Vector3& col0, Vector3& col1, Vector3& col2)
{
    m_values[0] = col0[0];
    m_values[1] = col1[0];
    m_values[2] = col2[0];

    m_values[3] = col0[1];
    m_values[4] = col1[1];
    m_values[5] = col2[1];

    m_values[6] = col0[2];
    m_values[7] = col1[3];
    m_values[8] = col2[2];
}

void Matrix3x3::SetSkewSymmetric(Vector3& v)
{
    // A 3x3 skew symmetric matrix can be used to represent cross products as matrix multiplications.
    m_values[0] = 0;
    m_values[1] = -v.z();
    m_values[2] = v.y();

    m_values[3] = v.z();
    m_values[4] = 0;
    m_values[5] = -v.x();

    m_values[6] = -v.y();
    m_values[7] = v.x();
    m_values[8] = 0;
}

Matrix3x3 Matrix3x3::Transpose() const
{
    return Matrix3x3(Column(0), Column(1), Column(2));
}

void Matrix3x3::DebugPrint()
{
    for (int i = 0; i < 3; i++)
    {
        printf("%f\t%f\t%f\n", m_values[i * 3], m_values[1 + i * 3], m_values[2 + i * 3]);
    }
}

Matrix3x3 Matrix3x3::Identity = Matrix3x3();

/*
* Define some helper functions for matrix inversion.
*/

static void swaprows3x3(Matrix3x3& a, int r1, int r2)
{
    //printf("Swapping row %d and %d\n", r1, r2);
    for (int col = 0; col < 3; col++)
    {
        float temp = a[r1][col];
        a[r1][col] = a[r2][col];
        a[r2][col] = temp;
    }
}

static void dividerow3x3(Matrix3x3& a, int r, float fac)
{
    a[r][0] /= fac;
    a[r][1] /= fac;
    a[r][2] /= fac;
}

static void submultrow3x3(Matrix3x3& a, int dest, int src, float fac)
{
    a[dest][0] -= fac * a[src][0];
    a[dest][1] -= fac * a[src][1];
    a[dest][2] -= fac * a[src][2];
}

//TODO this is copied from the CS488 supplied code. RE-WRITE!
/* ORIGINAL COMMENT FROM TA:
* invertMatrix
*
* I lifted this code from the skeleton code of a raytracer assignment
* from a different school.  I taught that course too, so I figured it
* would be okay.
*/


Matrix3x3 Matrix3x3::Inverse() const
{
    /* The algorithm is plain old Gauss-Jordan elimination
    with partial pivoting. */

    Matrix3x3 a(*this);
    Matrix3x3 ret;

    /* Loop over cols of a from left to right,
    eliminating above and below diag */

    /* Find largest pivot in column j among rows j..2 */
    for (int j = 0; j < 3; ++j)
    {
        int i1 = j; /* Row with largest pivot candidate */
        for (int i = j + 1; i < 3; ++i)
        {
            if (fabs(a[i][j]) > fabs(a[i1][j]))
            {
                i1 = i;
            }
        }

        /* Swap rows i1 and j in a and ret to put pivot on diagonal */
        swaprows3x3(a, i1, j);
        swaprows3x3(ret, i1, j);

        /* Scale row j to have a unit diagonal */
        if (a[j][j] == 0.0)
        {
            // Theoretically throw an exception.
            return ret;
        }

        dividerow3x3(ret, j, a[j][j]);
        dividerow3x3(a, j, a[j][j]);

        /* Eliminate off-diagonal elems in col j of a, doing identical
        ops to b */
        for (int i = 0; i < 3; ++i)
        {
            if (i != j)
            {
                submultrow3x3(ret, i, j, a[i][j]);
                submultrow3x3(a, i, j, a[i][j]);
            }
        }
    }

    return ret;
}

Vector3 operator *(const Matrix3x3& m, const Vector3& v)
{
    Vector3 ret;

    for (int i = 0; i < 3; ++i)
    {
        ret[i] = m[i][0] * v[0] +
                 m[i][1] * v[1] +
                 m[i][2] * v[2];
    }

    return ret;
}

Vector3 operator *(const Vector3& v, const Matrix3x3& m)
{
    Vector3 ret;

    for (int i = 0; i < 3; ++i)
    {
        ret[i] = m[0][i] * v[0] +
                 m[1][i] * v[1] +
                 m[2][i] * v[2];
    }

    return ret;
}

Matrix4x4::Matrix4x4()
{
    // default is identity matrix
    std::fill(m_values, m_values+m_size, 0.0f);
    m_values[0] = 1.0f;
    m_values[5] = 1.0f;
    m_values[10] = 1.0f;
    m_values[15] = 1.0f;
}

Matrix4x4::Matrix4x4(const Matrix4x4& other)
{
    //TODO use copy for Vectors/Points as well
    std::copy(other.m_values, other.m_values + m_size, m_values);
}

Matrix4x4::Matrix4x4(Vector4 row0, Vector4 row1, Vector4 row2, Vector4 row3)
{
    m_values[0] = row0[0];
    m_values[1] = row0[1];
    m_values[2] = row0[2];
    m_values[3] = row0[3];

    m_values[4] = row1[0];
    m_values[5] = row1[1];
    m_values[6] = row1[2];
    m_values[7] = row1[3];

    m_values[8] = row2[0];
    m_values[9] = row2[1];
    m_values[10] = row2[2];
    m_values[11] = row2[3];

    m_values[12] = row3[0];
    m_values[13] = row3[1];
    m_values[14] = row3[2];
    m_values[15] = row3[3];
}

Matrix4x4& Matrix4x4::operator =(const Matrix4x4& other)
{
    std::copy(other.m_values, other.m_values + m_size, m_values);
    return *this;
}

Vector4 Matrix4x4::operator[](int row) const
{
    return Row(row);
}
float* Matrix4x4::operator[](int row)
{
    return Row(row);
}

const float* Matrix4x4::Start() const
{
    return (float*)m_values;
}
const float* Matrix4x4::End() const
{
    return (float*)(m_values + m_size);
}

float* Matrix4x4::Row(int row)
{
    return (float*)m_values + 4 * row;
}

Vector4 Matrix4x4::Row(int row) const
{
    return Vector4(m_values[4*row], m_values[4*row+1], m_values[4*row+2], m_values[4*row+3]);
}

Vector4 Matrix4x4::Column(int col) const
{
    return Vector4(m_values[col], m_values[4+col], m_values[8+col], m_values[12+col]);
}

void Matrix4x4::SetColumns(Vector4& col0, Vector4& col1, Vector4& col2, Vector4& col3)
{
    m_values[0] = col0[0];
    m_values[1] = col1[0];
    m_values[2] = col2[0];
    m_values[3] = col3[0];

    m_values[4] = col0[1];
    m_values[5] = col1[1];
    m_values[6] = col2[1];
    m_values[7] = col3[1];

    m_values[8] = col0[2];
    m_values[9] = col1[2];
    m_values[10] = col2[2];
    m_values[11] = col3[2];

    m_values[12] = col0[3];
    m_values[13] = col1[3];
    m_values[14] = col2[3];
    m_values[15] = col3[3];
}

Matrix4x4 Matrix4x4::Transpose() const
{
    return Matrix4x4(Column(0), Column(1),
                     Column(2), Column(3));
}

Matrix4x4 Matrix4x4::Identity = Matrix4x4();

/*
 * Define some helper functions for matrix inversion.
 */

static void swaprows4x4(Matrix4x4& a, int r1, int r2)
{
    //printf("Swapping row %d and %d\n", r1, r2);
    for (int col = 0; col < 4; col++)
    {
        float temp = a[r1][col];
        a[r1][col] = a[r2][col];
        a[r2][col] = temp;
    }
}

static void dividerow4x4(Matrix4x4& a, int r, float fac)
{
    a[r][0] /= fac;
    a[r][1] /= fac;
    a[r][2] /= fac;
    a[r][3] /= fac;
}

static void submultrow4x4(Matrix4x4& a, int dest, int src, float fac)
{
    a[dest][0] -= fac * a[src][0];
    a[dest][1] -= fac * a[src][1];
    a[dest][2] -= fac * a[src][2];
    a[dest][3] -= fac * a[src][3];
}

//TODO this is copied from the CS488 supplied code. RE-WRITE!
/* ORIGINAL COMMENT FROM TA:
 * invertMatrix
 *
 * I lifted this code from the skeleton code of a raytracer assignment
 * from a different school.  I taught that course too, so I figured it
 * would be okay.
 */


Matrix4x4 Matrix4x4::Inverse() const
{
    /* The algorithm is plain old Gauss-Jordan elimination 
     with partial pivoting. */

    Matrix4x4 a(*this);
    Matrix4x4 ret;

    /* Loop over cols of a from left to right,
       eliminating above and below diag */

    /* Find largest pivot in column j among rows j..3 */
    for (int j = 0; j < 4; ++j)
    {
        int i1 = j; /* Row with largest pivot candidate */
        for (int i = j + 1; i < 4; ++i)
        {
            if (fabs(a[i][j]) > fabs(a[i1][j]))
            {
                i1 = i;
            }
        }

        /* Swap rows i1 and j in a and ret to put pivot on diagonal */
        swaprows4x4(a, i1, j);
        swaprows4x4(ret, i1, j);

        /* Scale row j to have a unit diagonal */
        if (a[j][j] == 0.0)
        {
            // Theoretically throw an exception.
            return ret;
        }

        dividerow4x4(ret, j, a[j][j]);
        dividerow4x4(a, j, a[j][j]);

        /* Eliminate off-diagonal elems in col j of a, doing identical
           ops to b */
        for (int i = 0; i < 4; ++i)
        {
            if (i != j)
            {
                submultrow4x4(ret, i, j, a[i][j]);
                submultrow4x4(a, i, j, a[i][j]);
            }
        }
    }

  return ret;
}

void Matrix4x4::DebugPrint()
{
    for (int i = 0; i < 4; i++)
    {
        printf("%f\t%f\t%f\t%f\n", m_values[i*4], m_values[1+i*4], m_values[2+i*4], m_values[3+i*4]);
    }
}

Matrix4x4 operator *(const Matrix4x4& a, const Matrix4x4& b)
{
    Matrix4x4 ret;

    for(int i = 0; i < 4; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            ret[i][j] = a[i][0] * b[0][j] +
                        a[i][1] * b[1][j] +
                        a[i][2] * b[2][j] +
                        a[i][3] * b[3][j];
        }
    }

    return ret;
}

Vector4 operator *(const Matrix4x4& m, const Vector4& v)
{
    Vector4 ret;

    for (int i = 0; i < 4; ++i)
    {
        ret[i] = m[i][0] * v[0] +
                 m[i][1] * v[1] +
                 m[i][2] * v[2] +
                 m[i][3] * v[3];
    }

    return ret;
}

Vector4 operator *(const Vector4& v, const Matrix4x4& m)
{
    Vector4 ret;

    for (int i = 0; i < 4; ++i)
    {
        ret[i] = m[0][i] * v[0] +
                 m[1][i] * v[1] +
                 m[2][i] * v[2] +
                 m[3][i] * v[3];
    }

    return ret;
}

Quaternion::Quaternion()
{
    *this = Identity;
}

Quaternion::Quaternion(const Quaternion& other)
{
    m_values[0] = other.m_values[0];
    m_values[1] = other.m_values[1];
    m_values[2] = other.m_values[2];
    m_values[3] = other.m_values[3];
}

Quaternion::Quaternion(float r, float i, float j, float k)
{
    m_values[0] = r;
    m_values[1] = i;
    m_values[2] = j;
    m_values[3] = k;
}

Quaternion& Quaternion::operator =(const Quaternion& other)
{
    m_values[0] = other.m_values[0];
    m_values[1] = other.m_values[1];
    m_values[2] = other.m_values[2];
    m_values[3] = other.m_values[3];
    return *this;
}

Quaternion& Quaternion::operator *=(const Quaternion& other)
{
    Quaternion q = *this;
    m_values[0] = q.r()*other.r() - q.i()*other.i() -
                  q.j()*other.j() - q.k()*other.k();
    m_values[1] = q.r()*other.i() + q.i()*other.r() +
                  q.j()*other.k() - q.k()*other.j();
    m_values[2] = q.r()*other.j() + q.j()*other.r() +
                  q.k()*other.i() - q.i()*other.k();
    m_values[3] = q.r()*other.k() + q.k()*other.r() +
                  q.i()*other.j() - q.j()*other.i();
    return *this;
}

float Quaternion::r() const
{
    return m_values[0];
}

float Quaternion::i() const
{
    return m_values[1];
}

float Quaternion::j() const
{
    return m_values[2];
}

float Quaternion::k() const
{
    return m_values[3];
}

void Quaternion::SetR(float r)
{
    m_values[0] = r;
}

void Quaternion::SetI(float i)
{
    m_values[1] = i;
}

void Quaternion::SetJ(float j)
{
    m_values[2] = j;
}

void Quaternion::SetK(float k)
{
    m_values[3] = k;
}

float Quaternion::MagnitudeSqrd() const
{
    return m_values[0] * m_values[0] +
           m_values[1] * m_values[1] +
           m_values[2] * m_values[2] +
           m_values[3] * m_values[3];
}

float Quaternion::Magnitude() const
{
    return sqrtf(MagnitudeSqrd());
}

void Quaternion::Normalize()
{
    float d = MagnitudeSqrd();

    if (Approximately(d, 0))
    {
        *this = Identity;
        return;
    }

    d = (float)1.0 / sqrtf(d);
    m_values[0] *= d;
    m_values[1] *= d;
    m_values[2] *= d;
    m_values[3] *= d;
}

void Quaternion::RotateByVector(const Vector3& v)
{
    Quaternion q(0, v.x(), v.y(), v.z());
    (*this) *= q;
}

void Quaternion::AddScaledVector(const Vector3& v, float s)
{
    Quaternion q(0, s*v.x(), s*v.y(), s*v.z());
    q *= *this;
    m_values[0] += 0.5f*q.r();
    m_values[1] += 0.5f*q.i();
    m_values[2] += 0.5f*q.j();
    m_values[3] += 0.5f*q.k();
}

Quaternion operator *(const Quaternion& a, const Quaternion& b)
{
    Quaternion ret = a;
    ret *= b;
    return ret;
}

Quaternion Quaternion::Identity = Quaternion(1, 0, 0, 0);

float DegreesToRadians(float degrees)
{
    return degrees * MATH_PI / 180.0f;
}

float RadiansToDegrees(float radians)
{
    return radians * 180.0f / MATH_PI;
}

float Clamp(float value, float min, float max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

// From http://floating-point-gui.de/errors/comparison/
bool Approximately(float a, float b)
{
    float absA = abs(a);
    float absB = abs(b);
    float diff = abs(a - b);

    float epsilon = std::numeric_limits<float>::epsilon();

    if (a == b)
    {
        return true;
    }
    else if (a == 0 || b == 0 || diff < FLT_MIN)        // TODO is this equivalent to min normal?
    {
        // a or b is zero or both are extremely close to it,
        // so relative error is less meaningful here
        return diff < (epsilon * FLT_MIN);
    }
    else
    {
        // Use relative error
        return (diff / std::min((absA + absB), FLT_MAX)) < epsilon;
    }
}

Vector3 AxisVector(eAXIS axis)
{
    switch(axis)
    {
    case AXIS_X:    return Vector3::Right;
    case AXIS_Y:    return Vector3::Up;
    case AXIS_Z:    return Vector3::Forward;
    default:        return Vector3::Zero;
    }
}