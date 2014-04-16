#pragma once

#include "..\GameComponent.h"

class Material;
class Mesh;

class MeshInstance : public GameComponent
{
public:
    MeshInstance(GameObject& gameObject);

    void Render(Matrix4x4& transform);

    void SetMesh(Mesh* mesh)                { m_mesh = mesh; }
    Mesh* GetMesh()                         { return m_mesh; }

    void SetMaterial(Material* material)    { m_material = material; }
    Material* GetMaterial()                 { return m_material; }

private:
    Mesh*       m_mesh;         // TODO should be shared through ResourceManager
    Material*   m_material;     // TODO should be shared through ResourceManager
};