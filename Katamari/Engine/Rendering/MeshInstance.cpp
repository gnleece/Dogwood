#include "MeshInstance.h"
#include "Mesh.h"

void MeshInstance::Render(Matrix4x4& transform)
{
    if (m_mesh)
    {
        m_mesh->Render(transform, m_material);
    }
}