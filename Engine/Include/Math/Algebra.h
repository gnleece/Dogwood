#pragma once

#include <cmath>
#include <memory>

#define MATH_PI 3.141592653589f

enum eAXIS { AXIS_X, AXIS_Y, AXIS_Z };

class Point3
{
public:
    Point3();
    Point3(const Point3& other);
    Point3(float x, float y, float z);
    Point3& operator =(const Point3 other);

    float x();
    float y();
    float z();

    void SetX(float x);
    void SetY(float y);
    void SetZ(float z);

private:
    float m_values[3];
};

class Vector2
{
public:
    Vector2();
    Vector2(const Vector2& other);
    Vector2(float x, float y);
    Vector2& operator =(const Vector2 other);
    bool operator ==(const Vector2 &other) const;

    float& operator[](int i);
    float operator[](int i) const;
    float* Start();

    float x();
    float y();

    void SetX(float x);
    void SetY(float y);

    void DebugPrint();

    static Vector2 Zero;
    static Vector2 One;

private:
    float m_values[2];
};

Vector2 operator +(const Vector2& a, const Vector2& b);
Vector2 operator -(const Vector2& a, const Vector2& b);

class Vector3
{
public:
    Vector3();
    Vector3(const Vector3& other);
    Vector3(float x, float y, float z);
    Vector3& operator =(const Vector3 other);
    Vector3& operator +=(const Vector3 other);
    Vector3& operator -=(const Vector3 other);
    bool operator ==(const Vector3 &other) const;

    float& operator[](int i);
    float operator[](int i) const;
    float* Start();

    float x();
    float y();
    float z();

    void SetX(float x);
    void SetY(float y);
    void SetZ(float z);

    float MagnitudeSqrd() const;
    float Magnitude() const;

    float Dot(const Vector3& other) const;
    Vector3 Cross(const Vector3& other) const;
    Vector3 Normalized() const;
    void Normalize();

    float MaxElement();
    float MinElement();

    Vector3 ComponentwiseInverse() const;
    bool HasZeroComponent() const;

    void DebugPrint();

    static Vector3 Zero;
    static Vector3 One;
    static Vector3 Right;
    static Vector3 Up;
    static Vector3 Forward;

private:
    float m_values[3];
};

Vector3 operator +(const Vector3& a, const Vector3& b);
Vector3 operator -(const Vector3& a, const Vector3& b);
Vector3 operator *(float s, const Vector3& v);
Vector3 operator *(const Vector3& v, float s);
Vector3 cross(const Vector3& a, const Vector3& b);
float dot(const Vector3& a, const Vector3& b);
Vector3 normalize(Vector3 v);

class Vector4
{
public:
    Vector4();
    Vector4(const Vector4& other);
    Vector4(float x, float y, float z, float w);
    Vector4(Vector3& v, float w);
    Vector4& operator =(const Vector4 other);

    float& operator[](int i);
    float operator[](int i) const;
    Vector3 xyz();

    float x();
    float y();
    float z();
    float w();

    void SetX(float x);
    void SetY(float y);
    void SetZ(float z);
    void SetW(float w);

    void DebugPrint();

private:
    float m_values[4];
};

Vector4 operator +(const Vector4& a, const Vector4& b);
Vector4 operator -(const Vector4& a, const Vector4& b);
Vector4 operator *(float s, const Vector4& v);

class Matrix4x4
{
public:
    Matrix4x4();
    Matrix4x4(const Matrix4x4& other);
    Matrix4x4(Vector4 row0, Vector4 row1, Vector4 row2, Vector4 row3);
    Matrix4x4 operator =(const Matrix4x4 other);

    Vector4 operator[](int row) const;
    float* operator[](int row);

    const float *Start() const;
    const float *End() const;

    Vector4 Row(int row) const;
    float* Row(int row);
    Vector4 Column(int col) const;

    Matrix4x4 Transpose() const;
    Matrix4x4 Inverse() const;

    void DebugPrint();

    static Matrix4x4 Identity;

private:
    static const int m_size = 16;
    float m_values[m_size];
};

//TODO matrix addition
Matrix4x4 operator *(const Matrix4x4& a, const Matrix4x4& b);

Vector4 operator *(const Matrix4x4& m, const Vector4& v);
Vector4 operator *(const Vector4& v, const Matrix4x4& m);

float DegreesToRadians(float degrees);
float RadiansToDegrees(float radians);

float Clamp(float value, float min, float max);
bool Approximately(float a, float b);