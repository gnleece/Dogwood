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

Mesh* MeshInstance::GetMesh()
{
    return m_mesh;
}

Material* MeshInstance::GetMaterial()
{
    return m_material;
}

void MeshInstance::SetMesh(Mesh* mesh)
{
    m_mesh = mesh;
}

void MeshInstance::SetMaterial(Material* material)
{
    m_material = material;
    if (material != NULL)
    {
        material->SetMesh(this);
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