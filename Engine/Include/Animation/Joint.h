#pragma once

#include <string>
#include "Math/Algebra.h"

using std::string;

struct Joint
{
	Matrix4x4		InverseJointPose;		// TODO Matrix4x3?
	string			Name;
	unsigned int	ParentIndex;
};

struct Skeleton
{
	unsigned int	JointCount;
	Joint*			Joints;
};

struct JointPose
{
	Quaternion		Rotation;
	Vector3			Translation;
	float			Scale;
};

struct SkeletonPose
{
	Skeleton*		Skeleton;
	JointPose*		LocalPose;
	Matrix4x4		GlobalPose;
};