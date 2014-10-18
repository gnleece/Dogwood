#include "Rendering\Material.h"
#include "Rendering\MeshInstance.h"
#include "Rendering\Mesh.h"

void MeshInstance::Render(Transform& transform, bool wireframe)
{
    if (m_mesh)
    {
        m_mesh->Render(transform, m_material, wireframe);
    }
}

MeshInstance* MeshInstance::DeepCopy()
{
    MeshInstance* newMeshInstance = new MeshInstance();
    newMeshInstance->SetMesh(m_mesh);

    if (m_material != NULL)
    {
        Material* newMaterial = m_material->DeepCopy();
        newMeshInstance->SetMaterial(newMaterial);
    }

    return newMeshInstance;
}