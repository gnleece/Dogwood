#include "Physics/Particles/ParticleContact.h"
#include "Physics/Particles/PhysicsParticle.h"

ParticleContact::ResolutionResult ParticleContact::Resolve(float deltaTime)
{
    ResolveVelocity(deltaTime);
    ResolutionResult result = ResolveInterpenetration(deltaTime);
    return result;
}

// Separating velocity (v_s) = (relative velocity) dot (contact normal) = (v_a - v_b) dot (norm(p_a - p_b))
float ParticleContact::CalculateSeparatingVelocity()
{
    Vector3 relativeVelocity = ParticleA->GetVelocity();
    if (ParticleB != NULL)
    {
        relativeVelocity -= ParticleB->GetVelocity();
    }
    return relativeVelocity.Dot(ContactNormal);
}

void ParticleContact::ResolveVelocity(float deltaTime)
{
    // Calculate the velocity in the direction of the contact
    float separatingVelocity = CalculateSeparatingVelocity();

    if (separatingVelocity > 0)
    {
        // The contact is either separating or stationary, so no impulse is required
        return;
    }

    // Use restitution to calculate the change in velocity
    float newSeparatingVelocity = -Restitution * separatingVelocity;

    // Check the velocity buildup due only to acceleration. This is to prevent issues with resting contacts.
    Vector3 accCausedVelocity = ParticleA->GetAcceleration();
    if (ParticleB != NULL)
    {
        accCausedVelocity -= ParticleB->GetAcceleration();
    }
    float accCausedSeparatingVelocity = accCausedVelocity.Dot(ContactNormal) * deltaTime;

    // If we have a closing velocity due to acceleration buildup, remove it from the new separating velocity
    if (accCausedSeparatingVelocity < 0)
    {
        newSeparatingVelocity += Restitution * accCausedSeparatingVelocity;
        if (newSeparatingVelocity < 0)
        {
            newSeparatingVelocity = 0;
        }
    }

    float deltaVelocity = newSeparatingVelocity - separatingVelocity;

    // Calculate the total inverse mass
    float totalInverseMass = ParticleA->GetInverseMass();
    if (ParticleB != NULL)
    {
        totalInverseMass += ParticleB->GetInverseMass();
    }

    if (totalInverseMass <= 0)
    {
        // Both particles has zero inverse mass == infinite mass => impulse has no effect
        return;
    }

    // Calculate the amount of impulse per unit of inverse mass
    // We apply the change in velocity in proportion to the inverse mass of each object
    // (Lower inverse mass == higher mass => lower change in velocity)
    float impulse = deltaVelocity / totalInverseMass;
    Vector3 impulsePerInverseMass = ContactNormal * impulse;

    // Apply impulses, in the direction of the contact
    Vector3 velocityA = ParticleA->GetVelocity() + impulsePerInverseMass * ParticleA->GetInverseMass();
    ParticleA->SetVelocity(velocityA);
    if (ParticleB != NULL)
    {
        // ParticleB has motion in the opposite direction from ParticleA
        Vector3 velocityB = ParticleB->GetVelocity() - impulsePerInverseMass * ParticleB->GetInverseMass();
        ParticleB->SetVelocity(velocityB);
    }
}

ParticleContact::ResolutionResult ParticleContact::ResolveInterpenetration(float deltaTime)
{
    if (Penetration <= 0)
    {
        // There's no interpenetration, so no resolution is required
        return ResolutionResult();
    }

    // Calculate the total inverse mass
    float totalInverseMass = ParticleA->GetInverseMass();
    if (ParticleB != NULL)
    {
        totalInverseMass += ParticleB->GetInverseMass();
    }

    if (totalInverseMass <= 0)
    {
        // Both particles has zero inverse mass == infinite mass => no resolution required
        return ResolutionResult();
    }

    // Calculate the movement amounts
    Vector3 movePerInverseMass = ContactNormal * (Penetration / totalInverseMass);
    ResolutionResult result;
    result.MovementA = movePerInverseMass * ParticleA->GetInverseMass();
    result.MovementB = Vector3::Zero;
    if (ParticleB != NULL)
    {
        result.MovementB = movePerInverseMass * -ParticleB->GetInverseMass();
    }

    // Apply penetration resolution
    Vector3 positionA = ParticleA->GetPosition() + result.MovementA;
    ParticleA->SetPosition(positionA);
    if (ParticleB != NULL)
    {
        Vector3 positionB = ParticleB->GetPosition() + result.MovementB;
        ParticleB->SetPosition(positionB);
    }

    return result;
}

ParticleContactResolver::ParticleContactResolver(unsigned int iterations)
{
    SetIterations(iterations);
}

void ParticleContactResolver::SetIterations(unsigned int iterations)
{
    m_iterations = iterations;
}

/* Resolve contacts one by one, starting with the one with the largest closing velocity. This first resolution
may change the closing velocity for other contacts, so check all contacts again after each resolution. 
Since resolving one contact could create another contact, we could end up with an infinite loop of endless contacts.
So, limit the total number of iterations (in practice this should be enough to resolve most contact sets in a 
believable way)*/
void ParticleContactResolver::ResolveContacts(ParticleContact* contacts, unsigned int numContacts, float deltaTime)
{
    unsigned int iterationsUsed = 0;
    while(iterationsUsed < m_iterations)
    {
        // Find the contact with the largest closing velocity (i.e. the smallest separating velocity)
        float max = FLT_MAX;
        unsigned int maxIndex = numContacts;
        for (unsigned int i = 0; i < numContacts; i++)
        {
            float separatingVelocity = contacts[i].CalculateSeparatingVelocity();
            if (separatingVelocity < max && (separatingVelocity < 0 || contacts[i].Penetration > 0))
            {
                max = separatingVelocity;
                maxIndex = i;
            }
        }

        // If we didn't find an index worth resolving, we're done
        if (maxIndex == numContacts)
            break;

        // Resolve the contact we chose
        ParticleContact::ResolutionResult result = contacts[maxIndex].Resolve(deltaTime);

        // Update the interpenetration for each particle
        for (unsigned int i = 0; i < numContacts; i++)
        {
            if (contacts[i].ParticleA == contacts[maxIndex].ParticleA)
            {
                contacts[i].Penetration -= result.MovementA.Dot(contacts[i].ContactNormal);
            }
            else if (contacts[i].ParticleA == contacts[maxIndex].ParticleB)
            {
                contacts[i].Penetration -= result.MovementB.Dot(contacts[i].ContactNormal);
            }

            if (contacts[i].ParticleB == NULL)
                continue;

            if (contacts[i].ParticleB == contacts[maxIndex].ParticleA)
            {
                contacts[i].Penetration += result.MovementA.Dot(contacts[i].ContactNormal);
            }
            else if (contacts[i].ParticleB == contacts[maxIndex].ParticleB)
            {
                contacts[i].Penetration += result.MovementB.Dot(contacts[i].ContactNormal);
            }
        }

        iterationsUsed++;
    }
}
