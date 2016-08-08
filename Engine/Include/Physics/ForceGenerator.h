#pragma once

#include "Math/Algebra.h"
#include <vector>

using std::vector;

class RigidBody;

class ForceGenerator
{
public:
    virtual void UpdateForce(RigidBody* body, float deltaTime) = 0;
};

class GravityGenerator : public ForceGenerator
{
public:
    GravityGenerator(Vector3& gravity);
    virtual void UpdateForce(RigidBody* body, float deltaTime);

private:
    Vector3 m_gravity;

};

class DragGenerator : public ForceGenerator
{
public:
    DragGenerator(float k1, float k2);
    virtual void UpdateForce(RigidBody* body, float deltaTime);

private:
    float   m_k1;
    float   m_k2;
};

class SpringGenerator : public ForceGenerator
{
public:
    SpringGenerator(RigidBody* other,
                    float springConstant,
                    float restLength,
                    Vector3& connectionPoint,
                    Vector3& otherConnectionPoint);
    virtual void UpdateForce(RigidBody* body, float deltaTime);

private:
    RigidBody*  m_other;
    float       m_springConstant;
    float       m_restLength;
    Vector3     m_connectionPoint;          // Given in local space
    Vector3     m_otherConnectionPoint;     // Given in local space of other object
};

class AnchoredSpringGenerator : public ForceGenerator
{
public:
    AnchoredSpringGenerator(Vector3& anchor, float springConstant, float restLength);
    virtual void UpdateForce(RigidBody* body, float deltaTime);

private:
    Vector3 m_anchor;
    float   m_sprintConstant;
    float   m_restLength;
};

class BungeeGenerator : public ForceGenerator
{
public:
    BungeeGenerator(RigidBody* other, float springConstant, float restLength);
    virtual void UpdateForce(RigidBody* body, float deltaTime);

private:
    RigidBody*  m_other;
    float       m_springConstant;
    float       m_restLength;
};

class BuoyancyGenerator : public ForceGenerator
{
public:
    BuoyancyGenerator(float maxDepth, float volume, float waterHeight, float liquidDensity = 1000.0f);
    virtual void UpdateForce(RigidBody* body, float deltaTime);

private:
    float   m_maxDepth;
    float   m_volume;
    float   m_waterHeight;
    float   m_liquidDensity;
};

class ForceRegistry
{
public:
    void    Register(RigidBody* body, ForceGenerator* generator);
    void    Unregister(RigidBody* body, ForceGenerator* generator);
    void    Clear();
    void    UpdateForces(float deltaTime);

protected:
    struct RegistrationInfo
    {
        RigidBody*      Body;
        ForceGenerator* Generator;

        RegistrationInfo(RigidBody* body, ForceGenerator* generator);
        bool operator==(const RegistrationInfo &other) const;
    };
    vector<RegistrationInfo> m_registry;
};