#include "Physics/BVHNode.h"
#include "Physics/BoundingSphere.h"
#include "Physics/CollisionEngine.h"

template<class BoundingVolumeType>
BVHNode<BoundingVolumeType>::BVHNode(BVHNode<BoundingVolumeType>* parent, Collider* collider, BoundingVolumeType& volume)
{
    m_parent = parent;
    m_collider = collider;
    m_volume = volume;

    m_children[0] = NULL;
    m_children[1] = NULL;
}

template<class BoundingVolumeType>
BVHNode<BoundingVolumeType>::~BVHNode()
{
    // Remove this node from the hierarchy. This deletes the node and all its children.
    // This also deletes the sibling node (if applicable) and moving that data up into the parent.

    // Process sibling (we have a sibling IFF we have a parent)
    if (m_parent != NULL)
    {
        // Find the sibling
        BVHNode<BoundingVolumeType>* sibling = NULL;
        if (m_parent->m_children[0] == this)
        {
            sibling = m_parent->m_children[1];
        }
        else
        {
            sibling = m_parent->m_children[2];
        }

        // Move the sibling's data up into the parent
        m_parent->m_volume = sibling->m_volume;
        m_parent->m_collider = sibling->m_collider;
        m_parent->m_children[0] = sibling->m_children[0];
        m_parent->m_children[1] = sibling->m_children[1];

        // Delete the sibling (reset its parent and children first to avoid processing their siblings in the delete)
        sibling->m_parent = NULL;
        sibling->m_collider = NULL;
        sibling->m_children[0] = NULL;
        sibling->m_children[1] = NULL;
        delete sibling;

        // Recalculate the parent's bounding volume
        m_parent->RecalculateBoundingVolume();
    }

    // Delete our children (reset their parent first to avoid processing their siblings in the delete)
    if (m_children[0] != NULL)
    {
        m_children[0]->m_parent = NULL;
        delete m_children[0];
    }
    if (m_children[1] != NULL)
    {
        m_children[1]->m_parent = NULL;
        delete m_children[1];
    }
}

template<class BoundingVolumeType>
unsigned int BVHNode<BoundingVolumeType>::GetPotentialContacts(PotentialContact* contacts, unsigned int limit)
{
    // Base case - we are a leaf node or have hit the max number of contacts
    if (IsLeaf() || limit == 0)
        return 0;

    // Otherwise, recurse on children
    return m_children[0]->GetPotentialContactsWith(contacts, limit, m_children[1]);
}

template<class BoundingVolumeType>
unsigned int BVHNode<BoundingVolumeType>::GetPotentialContactsWith(PotentialContact* contacts, unsigned int limit, BVHNode<BoundingVolumeType>* other)
{
    if (limit == 0)
        return 0;
        
    // If this region doesn't overlap with the other, there are no potential collisions
    if (!Overlaps(other))
        return 0;

    // Base case - If both regions are leaf nodes, we have a potential collision
    if (IsLeaf() && other->IsLeaf())
    {
        contacts->colliders[0] = m_collider;
        contacts->colliders[1] = other->m_collider;
        return 1;
    }

    // Determine which node to descend into:
    // If one region is a leaf, descend into the other.
    // Otherwise, descend into the node that has a larger volume
    BVHNode<BoundingVolumeType>* recursionNode = other;
    BVHNode<BoundingVolumeType>* nonRecursionNode = this;
    if (other->IsLeaf() || (!IsLeaf() && m_volume.GetSize() >= other->m_volume.GetSize()))
    {
        recursionNode = this;
        nonRecursionNode = other;
    }

    // Recurse:
    unsigned int count = recursionNode->m_children[0]->GetPotentialContactsWith(contacts, limit, nonRecursionNode);
    if (limit > count)
    {
        count += recursionNode->m_children[1]->GetPotentialContactsWith(contacts + count, limit - count, nonRecursionNode);
    }
    return count;
}

template<class BoundingVolumeType>
void BVHNode<BoundingVolumeType>::Insert(Collider* collider, BoundingVolumeType& volume)
{
    // If we are a leaf node, we need to create two new children and put the new body in one of them
    if (IsLeaf())
    {
        m_children[0] = new BVHNode<BoundingVolumeType>(this, m_collider, m_volume);
        m_children[1] = new BVHNode<BoundingVolumeType>(this, collider, volume);

        m_collider = NULL;          // We are no longer a leaf node, so clear our collider

        RecalculateBoundingVolume();
    }
    // Otherwise, we need to decide which child gets to keep the inserted collider.
    // We will give it to the child that would grow the least to incorporate it.
    else
    {
        if (m_children[0]->m_volume.GetGrowth(volume) < m_children[1]->m_volume.GetGrowth(volume))
        {
            m_children[0]->Insert(collider, volume);
        }
        else
        {
            m_children[1]->Insert(collider, volume);
        }
    }
}

template<class BoundingVolumeType>
BVHNode<BoundingVolumeType>* BVHNode<BoundingVolumeType>::Find(Collider* collider)
{
    if (m_collider == collider)
    {
        return this;
    }
        
    if (m_children[0] != NULL)
    {
        BVHNode<BoundingVolumeType>* firstChildResult = m_children[0]->Find(collider);
        if (firstChildResult != NULL)
        {
            return firstChildResult;
        }
    }

    if (m_children[1] != NULL)
    {
        BVHNode<BoundingVolumeType>* secondChildResult = m_children[1]->Find(collider);
        if (secondChildResult != NULL)
        {
            return secondChildResult;
        }
    }

    return NULL;
}

template<class BoundingVolumeType>
bool BVHNode<BoundingVolumeType>::IsLeaf()
{
    return m_collider != NULL;
}

template<class BoundingVolumeType>
bool BVHNode<BoundingVolumeType>::Overlaps(BVHNode<BoundingVolumeType>* other)
{
    return m_volume.Overlaps(&(other->m_volume));
}

template<class BoundingVolumeType>
void BVHNode<BoundingVolumeType>::RecalculateBoundingVolume()
{
    m_volume = BoundingSphere(m_children[0]->m_volume, m_children[1]->m_volume);
}

// Explicitly instantiate necessary type(s) here, so that we don't have to put all 
// templated function definitions in the header file.
template class BVHNode<BoundingSphere>;