#include "MeshInstance.h"
#include "Mesh.h"

void MeshInstance::Render(Transform& transform)
{
    if (m_mesh)
    {
        m_mesh->Render(transform, m_material);
    }
}