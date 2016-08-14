#include "Physics/PhysicsEngine.h"
#include "Physics/RigidBody.h"

void PhysicsEngine::Startup()
{
}

void PhysicsEngine::Shutdown()
{
}

void PhysicsEngine::StartFrame()
{
    // Reset accumulators for all rigid bodies
    vector<RigidBody*>::iterator iter;
    for (iter = m_rigidBodies.begin(); iter != m_rigidBodies.end(); iter++)
    {
        (*iter)->ClearAccumulators();
    }
}

void PhysicsEngine::Update(float deltaTime)
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
