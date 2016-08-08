#include "Physics/RigidBody.h"

RigidBody::RigidBody(GameObjectBase* gameObject) : m_gameObject(gameObject)
{

}

void RigidBody::SetInertiaTensor(Matrix3x3& inertiaTensor)
{
    m_inverseInertiaTensor = inertiaTensor.Inverse();
}

Vector3 RigidBody::GetPointInLocalSpace(const Vector3 &point)
{
    return m_transform.InverseTransformPoint(point);
}

Vector3 RigidBody::GetPointInWorldSpace(const Vector3 &point)
{
    return m_transform.TransformPoint(point);
}

Vector3 RigidBody::GetDirectionInLocalSpace(const Vector3 &direction)
{
    return m_transform.InverseTransformVector(direction);
}

Vector3 RigidBody::GetDirectionInWorldSpace(const Vector3 &direction)
{
    return m_transform.TransformVector(direction);
}

void RigidBody::Integrate(float deltaTime)
{
    ClearAccumulators();
}

void RigidBody::AddForce(Vector3& force)
{
    m_accumulatedForce += force;
    m_isAwake = true;
}

void RigidBody::AddForceAtPoint(Vector3& force, Vector3& point)
{
    // Convert to coordinates relative to center of mass
    Vector3 pt = point;
    pt -= m_position;

    m_accumulatedForce += force;
    m_accumulatedTorque += pt.Cross(force);

    m_isAwake = true;
}

//void RigidBody::AddForceAtPointLocalSpace(Vector3& force, Vector3& point)
//{
//     // TODO implement me
//}

void RigidBody::ClearAccumulators()
{
    m_accumulatedForce = Vector3::Zero;
    m_accumulatedTorque = Vector3::Zero;
}

void RigidBody::CalculateCachedData()
{
    m_rotation.Normalize();

    // Calculate the world transform
    CalculateTransform(m_position, m_rotation, m_transform.GetWorldMatrix());

    // Convert the inertia tensor from local space to world space
    TransformInertiaTensor(m_rotation,
                           m_inverseInertiaTensor,
                           m_transform.GetWorldMatrix(),
                           m_inverseInertiaTensorWorld);
}

// TODO move into math util?
// Formulas taken from Game Physics Engine Development pg. 210
void RigidBody::CalculateTransform(const Vector3& position, const Quaternion& rotation, Matrix4x4& transform)
{
    transform[0][0] = 1 - 2 * rotation.j()*rotation.j() - 2 * rotation.k()*rotation.k();
    transform[0][1] = 2 * rotation.i()*rotation.j() - 2 * rotation.r()*rotation.k();
    transform[0][2] = 2 * rotation.i()*rotation.k() + 2 * rotation.r()*rotation.j();
    transform[0][3] = position.x();

    transform[1][0] = 2 * rotation.i()*rotation.j() + 2 * rotation.r()*rotation.k();
    transform[1][1] = 1 - 2 * rotation.i()*rotation.i() - 2 * rotation.k()*rotation.k();
    transform[1][2] = 2 * rotation.k()*rotation.k() - 2 * rotation.r()*rotation.i();
    transform[1][3] = position.y();

    transform[2][0] = 2 * rotation.i()*rotation.k() - 2 * rotation.r()*rotation.j();
    transform[2][1] = 2 * rotation.j()*rotation.k() + 2 * rotation.r()*rotation.i();
    transform[2][2] = 1 - 2 * rotation.i()*rotation.i() - 2 * rotation.j()*rotation.j();
    transform[2][3] = position.z();
}

// Apply change of basis to convert (inverse) inertia tensor from local space to world space.
// We only need the rotational component of the transform, so we ignore the position as an optimization.
// Formulas taken from Game Physics Engine Development pg. 218
void RigidBody::TransformInertiaTensor(const Quaternion& q, const Matrix3x3& iitLocal, const Matrix4x4 transformWorld, Matrix3x3& iitWorld)
{
    float t4 = transformWorld[0][0]*iitLocal[0][0] +
               transformWorld[0][1]*iitLocal[1][0] +
               transformWorld[0][2]*iitLocal[2][0];

    float t9 = transformWorld[0][0] * iitLocal[0][1] +
               transformWorld[0][1] * iitLocal[1][1] +
               transformWorld[0][2] * iitLocal[2][1];

    float t14 = transformWorld[0][0] * iitLocal[0][2] +
                transformWorld[0][1] * iitLocal[1][2] +
                transformWorld[0][2] * iitLocal[2][2];

    float t28 = transformWorld[1][0] * iitLocal[0][0] +
                transformWorld[1][1] * iitLocal[1][0] +
                transformWorld[1][2] * iitLocal[2][0];

    float t33 = transformWorld[1][0] * iitLocal[0][1] +
                transformWorld[1][1] * iitLocal[1][1] +
                transformWorld[1][2] * iitLocal[2][1];

    float t38 = transformWorld[1][0] * iitLocal[0][2] +
                transformWorld[1][1] * iitLocal[1][2] +
                transformWorld[1][2] * iitLocal[2][2];

    float t52 = transformWorld[2][0] * iitLocal[0][0] +
                transformWorld[2][1] * iitLocal[1][0] +
                transformWorld[2][2] * iitLocal[2][0];

    float t57 = transformWorld[2][0] * iitLocal[0][1] +
                transformWorld[2][1] * iitLocal[1][1] +
                transformWorld[2][2] * iitLocal[2][1];

    float t62 = transformWorld[2][0] * iitLocal[0][2] +
                transformWorld[2][1] * iitLocal[1][2] +
                transformWorld[2][2] * iitLocal[2][2];

    iitWorld[0][0] = t4*transformWorld[0][0] + t9*transformWorld[0][1] + t14*transformWorld[0][2];
    iitWorld[0][1] = t4*transformWorld[1][0] + t9*transformWorld[1][1] + t14*transformWorld[1][2];
    iitWorld[0][2] = t4*transformWorld[2][0] + t9*transformWorld[2][1] + t14*transformWorld[2][2];
    iitWorld[1][0] = t28*transformWorld[0][0] + t33*transformWorld[0][1] + t38*transformWorld[0][2];
    iitWorld[1][1] = t28*transformWorld[1][0] + t33*transformWorld[1][1] + t38*transformWorld[1][2];
    iitWorld[1][2] = t28*transformWorld[2][0] + t33*transformWorld[2][1] + t38*transformWorld[2][2];
    iitWorld[2][0] = t52*transformWorld[0][0] + t57*transformWorld[0][1] + t62*transformWorld[0][2];
    iitWorld[2][1] = t52*transformWorld[1][0] + t57*transformWorld[1][1] + t62*transformWorld[1][2];
    iitWorld[2][2] = t52*transformWorld[2][0] + t57*transformWorld[2][1] + t62*transformWorld[2][2];
}