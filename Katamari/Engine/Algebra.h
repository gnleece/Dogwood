#pragma once

#include <cmath>
#include <memory>

#define M_PI 3.141592653589

enum E_AXIS { AXIS_X, AXIS_Y, AXIS_Z };

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

class Matrix4x4
{
public:
    Matrix4x4()
    {
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

private:
    static const int m_size = 16;
    float m_values[m_size];
};