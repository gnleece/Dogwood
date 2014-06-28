#include "Rendering\Vertex.h"


bool Vertex::operator==(const Vertex &other) const
{
    return (position == other.position &&
            normal == other.normal &&
            uv == other.uv);
}