#ifndef ENGINE_ALGEBRA_H
#define ENGINE_ALGEBRA_H

#include "stdafx.h"

#include <cmath>

enum E_AXIS { AXIS_X, AXIS_Y, AXIS_Z };

class Point3
{
public:
	Point3()
	{
		m_values[0] = 0.0;
		m_values[1] = 0.0;
		m_values[2] = 0.0;
	}
	Point3(const Point3& other)
	{
		m_values[0] = other[0];
		m_values[1] = other[1];
		m_values[2] = other[2];
	}
	Point3(float x, float y, float z)
	{
		m_values[0] = x;
		m_values[1] = y;
		m_values[2] = z;
	}

	Point3& operator =(const Point3 other)
	{
		m_values[0] = other[0];
		m_values[1] = other[1];
		m_values[2] = other[2];
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
		m_values[0] = 0.0;
		m_values[1] = 0.0;
		m_values[2] = 0.0;
	}
	Vector3(float x, float y, float z)
	{
		m_values[0] = x;
		m_values[1] = y;
		m_values[2] = z;
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

private:
	float m_values[3];
};

#endif

