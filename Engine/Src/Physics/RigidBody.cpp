#include "Physics/RigidBody.h"

RigidBody::RigidBody(GameObjectBase* gameObject) : m_gameObject(gameObject)
{

}

void RigidBody::SetInertiaTensor(Matrix3x3& inertiaTensor)
{
    m_inverseInertiaTensor = inertiaTensor.Inverse();
}

void RigidBody::CalculateCachedData()
{
    // TODO implement me, pg 210
}

// TODO move into math util
void RigidBody::CalculateTransform(Vector3& position, Quaternion& rotation, Matrix4x4& transform)
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