#include "Rendering\Material.h"
#include "Rendering\MeshInstance.h"
#include "Rendering\Mesh.h"

MeshInstance::MeshInstance()
{
    m_material = new Material();
    m_material->SetMesh(this);
}

MeshInstance::~MeshInstance()
{
    if (m_material != NULL)
    {
        delete m_material;
    }
}

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
    // clear old material
    if (m_material != NULL)
    {
        delete m_material;
    }

    // set new material
    m_material = material;
    if (m_material != NULL)
    {
        m_material->SetMesh(this);
    }
}

GameObjectBase* MeshInstance::GetGameObject()
{
    return m_gameObject;
}

void MeshInstance::SetGameObject(GameObjectBase* gameObject)
{
    m_gameObject = gameObject;
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