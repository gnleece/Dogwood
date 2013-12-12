#include "Algebra.h"

Point3::Point3()
{
    m_values[0] = 0.0f;
    m_values[1] = 0.0f;
    m_values[2] = 0.0f;
}

Point3::Point3(const Point3& other)
{
    m_values[0] = other.m_values[0];
    m_values[1] = other.m_values[1];
    m_values[2] = other.m_values[2];
}

Point3::Point3(float x, float y, float z)
{
    m_values[0] = x;
    m_values[1] = y;
    m_values[2] = z;
}

Point3& Point3::operator =(const Point3 other)
{
    m_values[0] = other.m_values[0];
    m_values[1] = other.m_values[1];
    m_values[2] = other.m_values[2];
    return *this;
}


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

Vector3& Vector3::operator =(const Vector3 other)
{
    m_values[0] = other.m_values[0];
    m_values[1] = other.m_values[1];
    m_values[2] = other.m_values[2];
    return *this;
}

float& Vector3::operator[](int i)
{
    return m_values[i];
}

float Vector3::operator[](int i) const
{
    return m_values[i];
}

float Vector3::MagnitudeSqrd() const
{
    return m_values[0]*m_values[0] +
           m_values[1]*m_values[1] +
           m_values[2]*m_values[2];
}

float Vector3::Magnitude() const
{
    return sqrt(MagnitudeSqrd());
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

// TODO (gnleece) make this more robust against floating point weirdness
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

Vector4& Vector4::operator =(const Vector4 other)
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

void Vector4::DebugPrint()
{
    printf("%f\t%f\t%f\t%f\n", m_values[0], m_values[1], m_values[2], m_values[3]);
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

Matrix4x4 Matrix4x4::operator =(const Matrix4x4 other)
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

Vector4 Matrix4x4::Row(int row) const
{
    return Vector4(m_values[4*row], m_values[4*row+1], m_values[4*row+2], m_values[4*row+3]);
}
float* Matrix4x4::Row(int row)
{
    return (float*)m_values + 4*row;
}

Vector4 Matrix4x4::Column(int col) const
{
    return Vector4(m_values[col], m_values[4+col], m_values[8+col], m_values[12+col]);
}

Matrix4x4 Matrix4x4::Transpose() const
{
    return Matrix4x4(Column(0), Column(1),
                     Column(2), Column(3));
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

/*
 * Define some helper functions for matrix inversion.
 */

static void swaprows(Matrix4x4& a, int r1, int r2)
{
    //printf("Swapping row %d and %d\n", r1, r2);
    for (int col = 0; col < 4; col++)
    {
        float temp = a[r1][col];
        a[r1][col] = a[r2][col];
        a[r2][col] = temp;
    }
}

static void dividerow(Matrix4x4& a, int r, float fac)
{
  a[r][0] /= fac;
  a[r][1] /= fac;
  a[r][2] /= fac;
  a[r][3] /= fac;
}

static void submultrow(Matrix4x4& a, int dest, int src, float fac)
{
  a[dest][0] -= fac * a[src][0];
  a[dest][1] -= fac * a[src][1];
  a[dest][2] -= fac * a[src][2];
  a[dest][3] -= fac * a[src][3];
}

//TODO this is copied from the CS488 supplied code. RE-WRITE MYSELF!
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
  for(int j = 0; j < 4; ++j) { 
    int i1 = j; /* Row with largest pivot candidate */
    for(int i = j + 1; i < 4; ++i) {
      if(fabs(a[i][j]) > fabs(a[i1][j])) {
        i1 = i;
      }
    }

    /* Swap rows i1 and j in a and ret to put pivot on diagonal */
    swaprows(a, i1, j);
    swaprows(ret, i1, j);

    /* Scale row j to have a unit diagonal */
    if(a[j][j] == 0.0) {
      // Theoretically throw an exception.
      return ret;
    }

    dividerow(ret, j, a[j][j]);
    dividerow(a, j, a[j][j]);

    /* Eliminate off-diagonal elems in col j of a, doing identical 
       ops to b */
    for(int i = 0; i < 4; ++i) {
      if(i != j) {
        submultrow(ret, i, j, a[i][j]);
        submultrow(a, i, j, a[i][j]);
      }
    }
  }

  return ret;
}

// based on pseudocode from 
// http://webglfactory.blogspot.com/2011/06/how-to-create-view-matrix.html
Matrix4x4 Matrix4x4::LookAt(Vector3 eye, Vector3 target, Vector3 up)
{
    Vector3 v_z = (eye - target).Normalized();
    Vector3 v_x = (cross(up, v_z)).Normalized();
    Vector3 v_y = cross(v_z, v_x);
    Vector3 w = Vector3(dot(v_x, eye), dot(v_y, eye), dot(v_z, eye));

    Matrix4x4 m = Matrix4x4(Vector4(v_x[0], v_y[0], v_z[0], w[0]),
                            Vector4(v_x[1], v_y[1], v_z[1], w[1]),
                            Vector4(v_x[2], v_y[2], v_z[2], w[2]),
                            Vector4(0, 0, 0, 1));

    return m;
}

Matrix4x4 Matrix4x4::Projection(float FOV, float aspect, float near, float far)
{
    float f = 1/tan(DegreesToRadians(FOV/2));
    return Matrix4x4(Vector4(f/aspect,0,0,0),
                     Vector4(0,f,0,0),
                     Vector4(0,0,(far+near)/(near-far),2*far*near/(near-far)),
                     Vector4(0,0,-1,0));
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

float DegreesToRadians(float degrees)
{
    return degrees*M_PI/180.0f;
}