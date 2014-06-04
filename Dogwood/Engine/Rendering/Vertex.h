#pragma once

#include "..\Math\Algebra.h"

class Vertex
{
public:
    Vertex() {}
    Vertex(Vector3 p, Vector3 n, Vector2 u) : position(p), normal(n), uv(u) {}

    bool operator==(const Vertex &other) const;

    Vector3 position;
    Vector3 normal;
    Vector2 uv;
};

namespace std
{
    template <> struct hash<Vertex>
    {
        size_t operator()(const Vertex & v) const
        {
            using std::size_t;
            using std::hash;

            // TODO this hash function is terrible, fix it
            return ((hash<float>()(v.position[0])
                ^ (hash<float>()(v.normal[1]) << 1)) >> 1)
                ^ (hash<float>()(v.uv[1]) << 1);
        }
    };
}