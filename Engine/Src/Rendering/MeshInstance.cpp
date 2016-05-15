#include "Rendering\Material.h"
#include "Rendering\MeshInstance.h"
#include "Rendering\Mesh.h"

#include "GameObject.h"
#include "Math\Transform.h"

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

int MeshInstance::GetTriangleCount()
{
    if (m_mesh == NULL)
        return 0;

    return m_mesh->GetTriangleCount();
}

void MeshInstance::GetTriangle(int index, Vector3* triangle, bool worldspace /* = false */)
{
    m_mesh->GetTriangle(index, triangle);

    if (worldspace)
    {
        // Transform each vertex of the triangle from model space to world space
        for (int i = 0; i < 3; i++)
        {
            triangle[i] = (m_gameObject->GetTransform().GetWorldMatrix() * Vector4(triangle[i], 1)).xyz();
        }
    }
}

float MeshInstance::GetBoundingRadius(bool worldspace /* = false */)
{
    float radius = m_mesh->GetBoundingRadius();

    if (worldspace)
    {
        // This is pretty hacky and gives a suboptimal radius, but it's good enough
        Vector3 scale = m_gameObject->GetTransform().GetWorldScale();
        float maxScale = std::fmaxf(scale[0], scale[1]);
        maxScale = std::fmaxf(maxScale, scale[2]);
        radius *= maxScale;
    }

    return radius;
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