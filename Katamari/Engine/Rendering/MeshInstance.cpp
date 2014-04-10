#include "MeshInstance.h"

MeshInstance::MeshInstance(GameObject& gameObject) : GameComponent(gameObject)
{ }

void MeshInstance::Render(Matrix4x4& transform)
{
    if (m_mesh)
    {
        m_mesh->Render(transform);
    }
}