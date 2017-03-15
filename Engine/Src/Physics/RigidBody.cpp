#include "Physics/RigidBody.h"

#include "GameObjectBase.h"
#include "Math/Transformations.h"
#include "Physics/Collider.h"
#include "Physics/ForceGenerator.h"
#include "Physics/PhysicsEngine.h"
#include "Serialization/HierarchicalSerializer.h"
#include <math.h>

RigidBody::RigidBody(GameObjectBase* gameObject)
    : m_gameObject(gameObject), m_isEnabled(true), m_mass(1.0f)
{ }

Vector3 RigidBody::GetPosition()
{
    return m_position;
}

void RigidBody::SetPosition(Vector3& position)
{
    m_position = position;
}

Quaternion& RigidBody::GetRotation()
{
    return m_rotation;
}

void RigidBody::SetRotation(Quaternion& rotation)
{
    m_rotation = rotation;
}

Vector3 RigidBody::GetVelocity()
{
    return m_velocity;
}

void RigidBody::SetVelocity(Vector3& velocity)
{
    m_velocity = velocity;
}

void RigidBody::AddVelocity(Vector3& deltaVelocity)
{
    m_velocity += deltaVelocity;
}

Vector3 RigidBody::GetAngularVelocity()
{
    return m_angularVelocity;
}

void RigidBody::SetAngularVelocity(Vector3& angularVelocity)
{
    m_angularVelocity = angularVelocity;
}

void RigidBody::AddAngularVelocity(Vector3& deltaAngularVelocity)
{
    m_angularVelocity += deltaAngularVelocity;
}

Vector3 RigidBody::GetAcceleration()
{
    return m_acceleration;
}

void RigidBody::SetAcceleration(Vector3& acceleration)
{
    m_acceleration = acceleration;
}

Vector3 RigidBody::GetPreviousAcceleration()
{
    return m_previousAcceleration;
}

float RigidBody::GetMass()
{
    if (!HasFiniteMass())
        return 0;

    return 1 / m_inverseMass;
}

void RigidBody::SetMass(float mass)
{
    if (mass < 0 || Approximately(mass, 0.f))
    {
        return;
    }
    m_inverseMass = 1.0f / mass;
    m_mass = mass;
}

float RigidBody::GetInverseMass()
{
    return m_inverseMass;
}

void RigidBody::SetInverseMass(float inverseMass)
{
    if (inverseMass < 0)
        return;
    m_inverseMass = inverseMass;
    m_mass = Approximately(inverseMass, 0.f) ? 0.f : 1 / inverseMass;
}

bool RigidBody::HasFiniteMass()
{
    return m_inverseMass > 0;
}

void RigidBody::SetInertiaTensor(Matrix3x3& inertiaTensor)
{
    m_inverseInertiaTensor = inertiaTensor.Inverse();
}

Matrix3x3& RigidBody::GetInverseIntertiaTensorWorld()
{
    return m_inverseInertiaTensorWorld;
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
    // Calculate linear acceleration from force inputs
    m_previousAcceleration = m_acceleration;
    m_previousAcceleration += m_accumulatedForce * m_inverseMass;

    // Calculate angular acceleration from torque inputs
    Vector3 angularAcceleration = m_inverseInertiaTensorWorld * m_accumulatedTorque;

    // Update linear velocity from both acceleration and impulse
    m_velocity += m_previousAcceleration * deltaTime;

    // Update angular velocity from both acceleration and impulse
    m_angularVelocity += angularAcceleration * deltaTime;

    // Impose drag
    m_velocity *= pow(LINEAR_DAMPING, deltaTime);
    m_angularVelocity *= pow(ANGULAR_DAMPING, deltaTime);

    // Update linear position
    m_position += m_velocity * deltaTime;

    // Update angular position
    m_rotation.AddScaledVector(m_angularVelocity, deltaTime);

    // Normalize rotation and update cached data
    CalculateCachedData();

    // Reset for next frame
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

RigidBody* RigidBody::Load(HierarchicalDeserializer* deserializer, GameObjectBase* gameObject)
{
    RigidBody* rigidBody = new RigidBody(gameObject);
    if (rigidBody != NULL)
    {
        rigidBody->Load(deserializer);
    }

    return rigidBody;
}

void RigidBody::AddToGameObject(GameObjectBase* gameObject, bool replaceExisiting)
{
    if (gameObject == NULL)
        return;

    RigidBody* previousBody = gameObject->GetRigidBody();
    if (previousBody == NULL || replaceExisiting)
    {
        RigidBody* rigidBody = new RigidBody(gameObject);
        gameObject->SetRigidBody(rigidBody);

        if (previousBody != NULL)
        {
            delete previousBody;
        }
    }
}

void RigidBody::Save(HierarchicalSerializer* serializer)
{
    serializer->PushScope("RigidBody");
    serializer->SetAttribute("IsEnabled", m_isEnabled);
    serializer->PopScope();
}

void RigidBody::Load(HierarchicalDeserializer* deserializer)
{
    bool isEnabled;
    deserializer->GetAttribute("IsEnabled", isEnabled);
    SetEnabled(isEnabled);
}

void RigidBody::SetEnabled(bool isEnabled)
{
    m_isEnabled = isEnabled;
}

bool RigidBody::IsEnabled()
{
    return m_isEnabled;
}

void RigidBody::OnCreate()
{
    PhysicsEngine::Singleton().RegisterRigidBody(this);

    // TODO temp hack
    if (strcmp(m_gameObject->GetName().c_str(), "Katamari") == 0)
    {
        SetMass(1.0f);

        // TODO allow custom gravity values
        // TODO re-use a single gravity generator?
        GravityGenerator* gravity = new GravityGenerator(Vector3(0.0f, -0.98f, 0.0f));
        PhysicsEngine::Singleton().RegisterForce(this, gravity);
    }

    // Calculate the inertia tensor     // TODO this should happen any time the colliders on the game object change
    Matrix3x3 inertiaTensor = Matrix3x3::Identity;
    std::vector<Collider*> colliders = m_gameObject->GetColliders();
    if (colliders.size() > 0)
    {
        // Use the first collider       // TODO handle objects with multiple colliders
        Collider* collider = colliders[0];
        inertiaTensor = collider->GetInertiaTensor(m_mass);
    }
    SetInertiaTensor(inertiaTensor);

    // Get position/rotation from gameobject transform
    m_position = m_gameObject->GetTransform().GetWorldPosition();
    m_rotation = EulerToQuaternion(m_gameObject->GetTransform().GetWorldRotation());
}

void RigidBody::UpdateGameObject()
{
    if (strcmp(m_gameObject->GetName().c_str(), "Katamari") == 0)
    {
        printf("Before: ");
        m_gameObject->GetTransform().GetWorldPosition().DebugPrint();
    }
    m_gameObject->GetTransform().SetWorldPosition(m_position);
    m_gameObject->GetTransform().SetWorldRotation(QuaternionToEuler(m_rotation));
    if (strcmp(m_gameObject->GetName().c_str(), "Katamari") == 0)
    {
        printf("After: ");
        m_position.DebugPrint();
        printf("-------\n");
    }
}

void RigidBody::ClearAccumulators()
{
    m_accumulatedForce = Vector3::Zero;
    m_accumulatedTorque = Vector3::Zero;

    CalculateCachedData();
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