#pragma once

#include <cmath>
#include <memory>

#define M_PI 3.141592653589

enum AXIS { AXIS_X, AXIS_Y, AXIS_Z };

class Point3
{
public:
    Point3()
    {
        m_values[0] = 0.0f;
        m_values[1] = 0.0f;
        m_values[2] = 0.0f;
    }
    Point3(const Point3& other)
    {
        m_values[0] = other.m_values[0];
        m_values[1] = other.m_values[1];
        m_values[2] = other.m_values[2];
    }
    Point3(float x, float y, float z)
    {
        m_values[0] = x;
        m_values[1] = y;
        m_values[2] = z;
    }

    Point3& operator =(const Point3 other)
    {
        m_values[0] = other.m_values[0];
        m_values[1] = other.m_values[1];
        m_values[2] = other.m_values[2];
        return *this;
    }

private:
    float m_values[3];
};

class Vector3
{
public:
    Vector3()
    {
        m_values[0] = 0.0f;
        m_values[1] = 0.0f;
        m_values[2] = 0.0f;
    }
    Vector3(float x, float y, float z)
    {
        m_values[0] = x;
        m_values[1] = y;
        m_values[2] = z;
    }

    float& operator[](int i)
    {
        return m_values[i];
    }
    float operator[](int i) const
    {
        return m_values[i];
    }

    float MagnitudeSqrd()
    {
        return m_values[0]*m_values[0] +
               m_values[1]*m_values[1] +
               m_values[2]*m_values[2];
    }

    float Magnitude()
    {
        return sqrt(MagnitudeSqrd());
    }

    float Dot(const Vector3& other) const
    {
        return m_values[0]*other.m_values[0] +
               m_values[1]*other.m_values[1] +
               m_values[2]*other.m_values[2];
    }

    Vector3 Cross(const Vector3& other) const
    {
        return Vector3(m_values[1]*other.m_values[2] - m_values[2]*other.m_values[1],
                       m_values[2]*other.m_values[0] - m_values[0]*other.m_values[2],
                       m_values[0]*other.m_values[1] - m_values[1]*other.m_values[0]);
    }

    bool Normalize();

private:
    float m_values[3];
};

inline Vector3 operator *(double s, const Vector3& v)
{
  return Vector3(s*v[0], s*v[1], s*v[2]);
}

inline Vector3 operator +(const Vector3& a, const Vector3& b)
{
  return Vector3(a[0]+b[0], a[1]+b[1], a[2]+b[2]);
}

inline Vector3 operator -(const Vector3& a, const Vector3& b)
{
  return Vector3(a[0]-b[0], a[1]-b[1], a[2]-b[2]);
}

inline Vector3 cross(const Vector3& a, const Vector3& b) 
{
  return a.Cross(b);
}

inline float dot(const Vector3& a, const Vector3& b)
{
    return a.Dot(b);
}

class Vector4
{
public:
    Vector4()
    {
        m_values[0] = 0.0f;
        m_values[1] = 0.0f;
        m_values[2] = 0.0f;
        m_values[3] = 0.0f;
    }
    Vector4(float x, float y, float z, float w)
    {
        m_values[0] = x;
        m_values[1] = y;
        m_values[2] = z;
        m_values[3] = w;
    }
    Vector4(Vector3& v, float w)
    {
        m_values[0] = v[0];
        m_values[1] = v[1];
        m_values[2] = v[2];
        m_values[3] = w;
    }

    float& operator[](int i)
    {
        return m_values[i];
    }
    float operator[](int i) const
    {
        return m_values[i];
    }

private:
    float m_values[4];
};

inline Vector4 operator *(double s, const Vector4& v)
{
  return Vector4(s*v[0], s*v[1], s*v[2], s*v[3]);
}

inline Vector4 operator +(const Vector4& a, const Vector4& b)
{
  return Vector4(a[0]+b[0], a[1]+b[1], a[2]+b[2], a[3]+b[3]);
}

inline Vector4 operator -(const Vector4& a, const Vector4& b)
{
  return Vector4(a[0]-b[0], a[1]-b[1], a[2]-b[2], a[3]-b[3]);
}

class Matrix4x4
{
public:
    Matrix4x4()
    {
        // default is identity matrix
        std::fill(m_values, m_values+m_size, 0.0f);
        m_values[0] = 1.0f;
        m_values[5] = 1.0f;
        m_values[10] = 1.0f;
        m_values[15] = 1.0f;
    }
    Matrix4x4(Vector4 row0, Vector4 row1, Vector4 row2, Vector4 row3)
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

    const float *Start() const
    {
        return (float*)m_values;
    }
    const float *End() const
    {
        return (float*)(m_values + m_size);
    }

    Vector4 getRow(size_t row) const
    {
    return Vector4(m_values[4*row], m_values[4*row+1], m_values[4*row+2], m_values[4*row+3]);
    }
    float *getRow(size_t row) 
    {
    return (float*)m_values + 4*row;
    }

    Vector4 operator[](size_t row) const
    {
    return getRow(row);
    }
    float *operator[](size_t row) 
    {
    return getRow(row);
    }

    //TODO quick hack fix this
    Vector4 getColumn(size_t col) const;
    Matrix4x4 Transpose() const;
    Matrix4x4 Invert() const;

    // TODO put these somewhere else maybe?
    static Matrix4x4 Rotation(float angle, AXIS axis);
    static Matrix4x4 Translation(const Vector3& displacement);
    static Matrix4x4 Scaling(const Vector3& scale);
    static Matrix4x4 LookAt(Vector3 eye, Vector3 target, Vector3 up);
    static Matrix4x4 Projection(float FOV, float aspect, float near, float far);

    void DebugPrint();

private:
    static const int m_size = 16;
    float m_values[m_size];
};

//TODO hack fix me
inline Matrix4x4 operator *(const Matrix4x4& a, const Matrix4x4& b)
{
  Matrix4x4 ret;

  for(size_t i = 0; i < 4; ++i) {
    Vector4 row = a.getRow(i);
		
    for(size_t j = 0; j < 4; ++j) {
      ret[i][j] = row[0] * b[0][j] + row[1] * b[1][j] + 
        row[2] * b[2][j] + row[3] * b[3][j];
    }
  }

  return ret;
}
float DegreesToRadians(float degrees);