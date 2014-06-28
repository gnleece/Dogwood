#include "Rendering\MeshInstance.h"
#include "Rendering\Mesh.h"

void MeshInstance::Render(Transform& transform)
{
    if (m_mesh)
    {
        m_mesh->Render(transform, m_material);
    }
}