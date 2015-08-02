#pragma once

#include "..\GameComponent.h"
#include "..\Math\Transform.h"

class Material;
class Mesh;

class MeshInstance : public GameComponent
{
public:
    void        Render(Transform& transform, bool wireframe = false);

    Mesh*       GetMesh();
    Material*   GetMaterial();

    void        SetMesh(Mesh* mesh);
    void        SetMaterial(Material* material);

    MeshInstance* DeepCopy();

private:
    Mesh*       m_mesh;
    Material*   m_material;
};