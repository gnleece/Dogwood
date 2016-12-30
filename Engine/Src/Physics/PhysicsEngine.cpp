#include "Physics/CollisionEngine.h"
#include "Physics/PhysicsEngine.h"
#include "Physics/RigidBody.h"

PhysicsEngine::PhysicsEngine() : m_contactResolver(MAX_RESOLUTION_ITERATIONS)
{}

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
    m_forceRegistry.UpdateForces(deltaTime);        // TODO unclear from textbook whether this should actually be here

    // Second, integrate all rigid bodies
    vector<RigidBody*>::iterator iter;
    for (iter = m_rigidBodies.begin(); iter != m_rigidBodies.end(); iter++)
    {
        (*iter)->Integrate(deltaTime);
    }
}

void PhysicsEngine::ResolveCollisions(float deltaTime)
{
    // TODO implement me
    //CollisionData cData = CollisionEngine::Singleton().GetCollisionData();
    //m_contactResolver.ResolveContacts(
    //    cData.Contacts,
    //    cData.contactCount,
    //    deltaTime
    //    );
}
