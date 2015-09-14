#pragma once

class Collider;

namespace DgwdPhysics
{
    struct PotentialContact
    {
        Collider* colliders[2];
    };

    class CollisionEngine
    {
    public:

        static CollisionEngine& Singleton()
        {
            static CollisionEngine singleton;
            return singleton;
        }
        CollisionEngine() {}

        void    Startup();
        void    Shutdown();

        void    Update(float deltaTime);

    private:
        void    BroadPhaseCollision();
    };
}