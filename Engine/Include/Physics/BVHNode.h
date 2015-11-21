#pragma once

//////////////////////////////////////////////////////////////////////////
// Node for a bounding volume hierarchy (BVH) which is a binary tree.
//////////////////////////////////////////////////////////////////////////

class Collider;
struct PotentialContact;

template<class BoundingVolumeType>
class BVHNode
{
public:
    BVHNode<BoundingVolumeType>(BVHNode<BoundingVolumeType>* parent, Collider* collider, BoundingVolumeType& volume);
    ~BVHNode();

    unsigned int        GetPotentialContacts(PotentialContact* contacts, unsigned int limit);
    void                Insert(Collider* collider, BoundingVolumeType& volume);
    BVHNode<BoundingVolumeType>*            Find(Collider* collider);

private:
    unsigned int        GetPotentialContactsWith(PotentialContact* contacts, unsigned int limit, BVHNode<BoundingVolumeType>* other);
    bool                IsLeaf();
    bool                Overlaps(BVHNode<BoundingVolumeType>* other);
    void                RecalculateBoundingVolume();

    BVHNode<BoundingVolumeType>*            m_parent;
    BVHNode<BoundingVolumeType>*            m_children[2];
    BoundingVolumeType  m_volume;
    Collider*           m_collider;         // This will be NULL for all non-leaf nodes
};