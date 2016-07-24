#pragma once

#include "Math/Algebra.h"

class GameObjectBase;

class RigidBody
{
public:
    RigidBody(GameObjectBase* gameObject);

    void        SetPosition(Vector3 position);
    Vector3     GetPosition();

    void        SetVelocity(Vector3 velocity);
    Vector3     GetVelocity();

    void        SetAcceleration(Vector3 acceleration);
    Vector3     GetAcceleration();

    void        SetMass(float mass);
    void        SetInverseMass(float inverseMass);
    float       GetMass();
    float       GetInverseMass();
    bool        HasFiniteMass();

    void        SetInertiaTensor(Matrix3x3& inertiaTensor);

    // Integrates the particle forward in time (i.e. updates position and velocity)
    void        Integrate(float deltaTime);

    // Adds the given force to the particle to be applied at the next integration only
    void        AddForce(Vector3 force);

    void        ClearAccumulator();

protected:
    void        CalculateCachedData();
    void        CalculateTransform(const Vector3& position, const Quaternion& rotation, Matrix4x4& transform);
    void        TransformInertiaTensor(const Quaternion& q, const Matrix3x3& iitLocal, const Matrix4x4 transformWorld, Matrix3x3& iitWorld);

    GameObjectBase* m_gameObject;

    // Transform values are in world space
    Vector3         m_position;
    Quaternion      m_rotation;
    Vector3         m_velocity;
    Vector3         m_angularVelocity;
    Vector3         m_acceleration;

    Matrix4x4       m_transformMatrix;      // TODO reconcile with go transform

    // Inverse mass is used here instead of simply mass, so that we can represent "infinite" mass (with an
    // inverse mass of 0), and *cannot* represent zero mass (which we don't want to allow).
    float           m_inverseMass;

    // This is calculated from the inverse mass, and then cached. It is not set directly.
    float           m_mass;

    // Inverse inertia tensor is used here for similar reasons to why inverse mass is used (see above).
    // Given in local space.
    Matrix3x3       m_inverseInertiaTensor;

    // IIT in world space (calculated and cached)
    Matrix3x3       m_inverseInertiaTensorWorld;

    Vector3         m_accumulatedForce;

    // Damping is required to remove energy added from numerical instability in physics integration step.
    const float     DAMPING = 0.999f;
};