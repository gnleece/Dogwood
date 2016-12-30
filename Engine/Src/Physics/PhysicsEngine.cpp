#include "Physics/CollisionEngine.h"
#include "Physics/PhysicsEngine.h"
#include "Physics/RigidBody.h"

#include <algorithm>

PhysicsEngine::PhysicsEngine() : m_contactResolver(MAX_RESOLUTION_ITERATIONS)
{
}

void PhysicsEngine::Startup()
{
}

void PhysicsEngine::Shutdown()
{
}

// TODO remove?
//void PhysicsEngine::StartFrame()
//{
//    // Reset accumulators for all rigid bodies
//    vector<RigidBody*>::iterator iter;
//    for (iter = m_rigidBodies.begin(); iter != m_rigidBodies.end(); iter++)
//    {
//        (*iter)->ClearAccumulators();
//    }
//}

void PhysicsEngine::UpdateBodies(float deltaTime)
{
    // First, apply force generators
    m_forceRegistry.UpdateForces(deltaTime);

    // Second, integrate all rigid bodies
    vector<RigidBody*>::iterator iter;
    for (iter = m_rigidBodies.begin(); iter != m_rigidBodies.end(); iter++)
    {
        (*iter)->Integrate(deltaTime);
    }
}

void PhysicsEngine::ResolveCollisions(float deltaTime)
{
    // Get all collision data from collision engine
    const CollisionData* collisionData = CollisionEngine::Singleton().GetCollisionData();

    // Determine which collisions involve objects with rigid bodies
    int contactCount = 0;
    for (int i = 0; i < collisionData->ContactsUsed; i++)
    {
        if (contactCount >= MAX_RIGID_BODY_CONTACTS)
            break;

        CollisionContact collisionContact = collisionData->Contacts[i];
        // TODO finish implementation
    }

    // Resolve the collisions involving rigid bodies
    m_contactResolver.ResolveContacts(m_rigiBodyContacts, contactCount, deltaTime);
}

void PhysicsEngine::RegisterRigidBody(RigidBody* rigidBody)
{
    if (rigidBody == NULL)
        return;

    // TODO make sure this rigidbody isn't already in the list
    m_rigidBodies.push_back(rigidBody);
}

void PhysicsEngine::UnregisterRigidBody(RigidBody* rigidBody)
{
    m_rigidBodies.erase(
        std::remove(m_rigidBodies.begin(), m_rigidBodies.end(), rigidBody),
        m_rigidBodies.end());
}