#pragma once

#include "..\GameComponent.h"
#include "Mesh.h"

class MeshInstance : public GameComponent
{
public:
    MeshInstance(GameObject& gameObject);

    void Render(Matrix4x4& transform);

    void SetMesh(Mesh* mesh)    { m_mesh = mesh; }
    Mesh* GetMesh()             { return m_mesh; }

private:
    Mesh* m_mesh;    // TODO should be shared through ResourceManager
};