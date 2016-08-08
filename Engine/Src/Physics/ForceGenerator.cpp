#include "Physics/ForceGenerator.h"
#include "Physics/RigidBody.h"
#include <algorithm>

GravityGenerator::GravityGenerator(Vector3& gravity) : m_gravity(gravity)
{ }

void GravityGenerator::UpdateForce(RigidBody* body, float deltaTime)
{
    if (!body->HasFiniteMass())
        return;

    // Apply the mass-scaled force to the body
    body->AddForce(m_gravity * body->GetMass());
}

//-----------------------------------------------------------------------------------------------

DragGenerator::DragGenerator(float k1, float k2) : m_k1(k1), m_k2(k2)
{ }

void DragGenerator::UpdateForce(RigidBody* body, float deltaTime)
{
    Vector3 force = body->GetVelocity();

    // Calculate the total drag coefficient
    float dragCoeff = force.Magnitude();
    dragCoeff = (m_k1 * dragCoeff) + (m_k2 * dragCoeff * dragCoeff);

    // Calculate the final force and apply it
    force.Normalize();
    force = force * -dragCoeff;
    body->AddForce(force);
}

//-----------------------------------------------------------------------------------------------

SpringGenerator::SpringGenerator(RigidBody* other,
                                 float springConstant,
                                 float restLength,
                                 Vector3& connectionPoint,
                                 Vector3& otherConnectionPoint)
    : m_other(other), m_springConstant(springConstant), m_restLength(restLength),
      m_connectionPoint(connectionPoint), m_otherConnectionPoint(otherConnectionPoint)
{ }

void SpringGenerator::UpdateForce(RigidBody* body, float deltaTime)
{
    // Convert connection point positions to world space
    Vector3 posA = body->GetPointInWorldSpace(m_connectionPoint);
    Vector3 posB = m_other->GetPointInWorldSpace(m_otherConnectionPoint);

    // Calculate the vector of the spring
    Vector3 force = body->GetPosition() - m_other->GetPosition();

    // Calculate the magnitude of the force
    float magnitude = force.Magnitude();
    magnitude = abs(magnitude - m_restLength);
    magnitude *= m_springConstant;

    // Calculate the final force and apply it
    force.Normalize();
    force = force * -magnitude;
    body->AddForce(force);
}

//-----------------------------------------------------------------------------------------------

AnchoredSpringGenerator::AnchoredSpringGenerator(Vector3& anchor, float springConstant, float restLength)
    : m_anchor(anchor), m_sprintConstant(springConstant), m_restLength(restLength)
{ }

void AnchoredSpringGenerator::UpdateForce(RigidBody* body, float deltaTime)
{
    // Calculate the vector of the spring
    Vector3 force = body->GetPosition() - m_anchor;

    // Calculate the magnitude of the force
    float magnitude = force.Magnitude();
    magnitude = (m_restLength - magnitude) * m_sprintConstant;

    // Calculate the final force and apply it
    force.Normalize();
    force = force * magnitude;
    body->AddForce(force);
}

//-----------------------------------------------------------------------------------------------

BungeeGenerator::BungeeGenerator(RigidBody* other, float springConstant, float restLength)
    : m_other(other), m_springConstant(springConstant), m_restLength(restLength)
{ }

void BungeeGenerator::UpdateForce(RigidBody* body, float deltaTime)
{
    // Calculate the vector of the spring
    Vector3 force = body->GetPosition() - m_other->GetPosition();

    // Check if the bungee is compressed
    float magnitude = force.Magnitude();
    if (magnitude <= m_restLength)
        return;

    // Calculate the magnitude of the force
    magnitude = (m_restLength * magnitude) * m_springConstant;

    // Calculate the final force and apply it
    force.Normalize();
    force = force * -magnitude;
    body->AddForce(force);
}

//-----------------------------------------------------------------------------------------------

BuoyancyGenerator::BuoyancyGenerator(float maxDepth, float volume, float waterHeight, float liquidDensity /* = 1000.0f */)
    : m_maxDepth(maxDepth), m_volume(volume), m_waterHeight(waterHeight), m_liquidDensity(liquidDensity)
{ }

void BuoyancyGenerator::UpdateForce(RigidBody* body, float deltaTime)
{
    // Calculate the submersion depth
    float depth = body->GetPosition().y();

    // Check if we're out of the water
    if (depth >= m_waterHeight + m_maxDepth)
        return;

    // Check if we're at maximum depth
    if (depth <= m_waterHeight - m_maxDepth)
    {
        Vector3 force;
        force.SetY(m_liquidDensity * m_volume);
        body->AddForce(force);
    }
    // Otherwise we are partly submerged
    else
    {
        Vector3 force;
        force.SetY(m_liquidDensity * m_volume * (depth - m_maxDepth - m_waterHeight) / 2 * m_maxDepth);
        body->AddForce(force);
    }
}

//-----------------------------------------------------------------------------------------------

void ForceRegistry::Register(RigidBody* body, ForceGenerator* generator)
{
    RegistrationInfo info(body, generator);
    m_registry.push_back(info);
}

void ForceRegistry::Unregister(RigidBody* body, ForceGenerator* generator)
{
    RegistrationInfo info(body, generator);
    m_registry.erase(
        std::remove(m_registry.begin(), m_registry.end(), info),
        m_registry.end());
}

void ForceRegistry::Clear()
{
    m_registry.clear();
}

void ForceRegistry::UpdateForces(float deltaTime)
{
    vector<RegistrationInfo>::iterator iter;
    for (iter = m_registry.begin(); iter != m_registry.end(); iter++)
    {
        iter->Generator->UpdateForce(iter->Body, deltaTime);
    }
}

ForceRegistry::RegistrationInfo::RegistrationInfo(RigidBody* body, ForceGenerator* generator)
{
    Body = body;
    Generator = generator;
}

bool ForceRegistry::RegistrationInfo::operator==(const RegistrationInfo &other) const
{
    return Body == other.Body && Generator == other.Generator;
}