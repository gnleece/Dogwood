#include "Physics/PhysicsEngine.h"

#include "GameObjectBase.h"
#include "GameProject.h"
#include "Physics/Collider.h"
#include "Physics/CollisionDetection.h"
#include "Physics/CollisionEngine.h"
#include "Physics/RigidBody.h"
#include "Physics/RigidBodyContact.h"

#include <algorithm>

PhysicsEngine::PhysicsEngine() : m_contactResolver(MAX_RESOLUTION_ITERATIONS)
{
}

void PhysicsEngine::Startup()
{
    float gravityAmt = GameProject::Singleton().GetPhysicsSettings().Gravity;
    m_gravityGenerator = new GravityGenerator(Vector3(0.0f, gravityAmt, 0.0f));
}

void PhysicsEngine::Shutdown()
{
    delete m_gravityGenerator;
}

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
        GameObjectBase* objectA = collisionContact.ColliderA->GetGameObject();
        GameObjectBase* objectB = collisionContact.ColliderB->GetGameObject();
        RigidBody* rigidBodyA = objectA->GetRigidBody();
        RigidBody* rigidBodyB = objectB->GetRigidBody();

        if (rigidBodyA == NULL && rigidBodyB == NULL)
        {
            // No rigid body in this collision, so do nothing
            continue;
        }

        // Create a rigid body contact, since there is at least one rigid body involved
        // One of the bodies might be NULL, but RigidBodyContact will handle this case
        m_rigiBodyContacts[contactCount].Body[0] = rigidBodyA;
        m_rigiBodyContacts[contactCount].Body[1] = rigidBodyB;
        m_rigiBodyContacts[contactCount].ContactPoint = collisionContact.ContactPoint;
        m_rigiBodyContacts[contactCount].ContactNormal = collisionContact.ContactNormal;
        m_rigiBodyContacts[contactCount].Penetration = collisionContact.Penetration;
        m_rigiBodyContacts[contactCount].Friction = 0.9f;       // TODO custom friction values
        m_rigiBodyContacts[contactCount].Restitution = 0.1f;    // TODO custom restitution values
        contactCount++;
    }

    // Resolve the collisions involving rigid bodies
    if (contactCount > 0)
    {
        m_contactResolver.ResolveContacts(m_rigiBodyContacts, contactCount, deltaTime);
    }

    // TODO this shouldn't go here
    // Update gameobject transforms to match rigidbody positions/rotations
    vector<RigidBody*>::iterator iter;
    for (iter = m_rigidBodies.begin(); iter != m_rigidBodies.end(); iter++)
    {
        (*iter)->UpdateGameObject();
    }
}

void PhysicsEngine::RegisterRigidBody(RigidBody* rigidBody)
{
    if (rigidBody == NULL)
        return;

    vector<RigidBody*>::iterator iter;
    iter = std::find(m_rigidBodies.begin(), m_rigidBodies.end(), rigidBody);
    if (iter != m_rigidBodies.end())
    {
        // This rigid body is already registered
        // TODO assert or log a warning
        return;
    }

    m_rigidBodies.push_back(rigidBody);
}

void PhysicsEngine::UnregisterRigidBody(RigidBody* rigidBody)
{
    m_rigidBodies.erase(
        std::remove(m_rigidBodies.begin(), m_rigidBodies.end(), rigidBody),
        m_rigidBodies.end());
}

void PhysicsEngine::RegisterForce(RigidBody* rigidBody, ForceGenerator* forceGenerator)
{
    m_forceRegistry.Register(rigidBody, forceGenerator);
}

void PhysicsEngine::UnregisterForce(RigidBody* rigidBody, ForceGenerator* forceGenerator)
{
    m_forceRegistry.Unregister(rigidBody, forceGenerator);
}

GravityGenerator* PhysicsEngine::GetGravityGenerator()
{
    return m_gravityGenerator;
}