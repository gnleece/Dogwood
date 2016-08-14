#pragma once

#include <vector>
#include "Physics/ForceGenerator.h"

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
    PhysicsEngine() {}

    void    Startup();
    void    Shutdown();

    void    StartFrame();
    void    Update(float deltaTime);

private:
    vector<RigidBody*>  m_rigidBodies;
    ForceRegistry       m_forceRegistry;
};