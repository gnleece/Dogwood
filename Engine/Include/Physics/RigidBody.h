#pragma once

#include "Math/Algebra.h"
#include "Math/Transform.h"

class GameObjectBase;

class RigidBody
{
public:
    RigidBody(GameObjectBase* gameObject);

    void        SetPosition(Vector3 position);
    Vector3     GetPosition();

    void        SetRotation(Quaternion& rotation);
    Quaternion& GetRotation();

    void        SetVelocity(Vector3 velocity);
    Vector3     GetVelocity();

    void        SetAngularVelocity(Vector3 angularVelocity);
    Vector3     GetAngularVelocity();

    void        SetAcceleration(Vector3 acceleration);
    Vector3     GetAcceleration();

    void        SetMass(float mass);
    void        SetInverseMass(float inverseMass);
    float       GetMass();
    float       GetInverseMass();
    bool        HasFiniteMass();

    void        SetInertiaTensor(Matrix3x3& inertiaTensor);

    Vector3     GetPointInLocalSpace(const Vector3 &point);
    Vector3     GetPointInWorldSpace(const Vector3 &point);
    Vector3     GetDirectionInLocalSpace(const Vector3 &direction);
    Vector3     GetDirectionInWorldSpace(const Vector3 &direction);
    Matrix3x3&  GetInverseIntertiaTensorWorld();

    // Integrates the particle forward in time (i.e. updates position and velocity)
    void        Integrate(float deltaTime);

    // "AddForce" functions apply and next integration only

    // Adds the given force to the particle, applied at the center of mass
    void        AddForce(Vector3& force);

    // Adds the given force to the given point on the rigid body. Both values given in world space.
    // Because the force is not applied at the center of mass, it may be split into force and torque.
    void        AddForceAtPoint(Vector3& force, Vector3& point);

    // Adds the given force to the given point on the rigid body. Force given in world space and point in local space.
    // Because the force is not applied at the center of mass, it may be split into force and torque.
    //void        AddForceAtPointLocalSpace(Vector3& force, Vector3& point);

    void        ClearAccumulators();

protected:
    void        CalculateCachedData();
    void        CalculateTransform(const Vector3& position, const Quaternion& rotation, Matrix4x4& transform);
    void        TransformInertiaTensor(const Quaternion& q, const Matrix3x3& iitLocal, const Matrix4x4 transformWorld, Matrix3x3& iitWorld);

    GameObjectBase* m_gameObject;
    bool            m_isAwake;

    // Transform values are in world space
    Vector3         m_position;
    Quaternion      m_rotation;
    Vector3         m_velocity;
    Vector3         m_angularVelocity;
    Vector3         m_acceleration;
    Vector3         m_previousAcceleration;

    Transform       m_transform;      // TODO reconcile with go transform

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
    Vector3         m_accumulatedTorque;

    // Damping is required to remove energy added from numerical instability in physics integration step.
    const float     LINEAR_DAMPING = 0.999f;
    const float     ANGULAR_DAMPING = 0.999f;
};