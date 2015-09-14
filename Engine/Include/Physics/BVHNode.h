#pragma once

//////////////////////////////////////////////////////////////////////////
// Node for a bounding volume hierarchy (BVH) which is a binary tree.
//////////////////////////////////////////////////////////////////////////

namespace DgwdPhysics
{
    class Collider;
    struct PotentialContact;

    template<class BoundingVolumeType>
    class BVHNode
    {
    public:
        BVHNode(BVHNode* parent, Collider* collider, BoundingVolumeType& volume);
        ~BVHNode();

        unsigned int        GetPotentialContacts(PotentialContact* contacts, unsigned int limit);
        void                Insert(Collider* collider, BoundingVolumeType& volume);

    private:
        unsigned int        GetPotentialContactsWith(PotentialContact* contacts, unsigned int limit, BVHNode<BoundingVolumeType>* other);
        bool                IsLeaf();
        bool                Overlaps(BVHNode<BoundingVolumeType>* other);
        void                RecalculateBoundingVolume();

        BVHNode*            m_parent;
        BVHNode*            m_children[2];
        BoundingVolumeType  m_volume;
        Collider*           m_collider;         // This will be NULL for all non-leaf nodes
    };
}