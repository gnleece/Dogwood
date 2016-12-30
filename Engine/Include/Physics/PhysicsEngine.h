#pragma once

#include <vector>
#include "Physics/ForceGenerator.h"
#include "Physics/RigidBodyContact.h"

#define MAX_RESOLUTION_ITERATIONS 25
#define MAX_RIGID_BODY_CONTACTS 25

using std::vector;

class RigidBody;

class PhysicsEngine
{
public:

    static PhysicsEngine& Singleton()
    {
        static PhysicsEngine singleton;
        return singleton;
    }
    PhysicsEngine();

    void    Startup();
    void    Shutdown();

    //void    StartFrame();
    void    UpdateBodies(float deltaTime);
    void    ResolveCollisions(float deltaTime);

    void    RegisterRigidBody(RigidBody* rigidBody);
    void    UnregisterRigidBody(RigidBody* rigidBody);

private:
    vector<RigidBody*>  m_rigidBodies;
    ForceRegistry       m_forceRegistry;
    ContactResolver     m_contactResolver;
    RigidBodyContact    m_rigiBodyContacts[MAX_RIGID_BODY_CONTACTS];
};