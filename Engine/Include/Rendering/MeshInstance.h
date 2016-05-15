#pragma once

#include "..\GameComponent.h"
#include "..\Math\Transform.h"

class GameObjectBase;
class Material;
class Mesh;

class MeshInstance
{
public:
    MeshInstance();
    ~MeshInstance();

    void            Render(Transform& transform, bool wireframe = false);

    Mesh*           GetMesh();
    Material*       GetMaterial();
    void            SetMesh(Mesh* mesh);
    void            SetMaterial(Material* material);

    GameObjectBase* GetGameObject();
    void            SetGameObject(GameObjectBase* gameObject);

    int             GetTriangleCount();
    void            GetTriangle(int index, Vector3* triangle, bool worldspace = false);

    float           GetBoundingRadius(bool worldspace = false);

    MeshInstance*   DeepCopy();

private:
    Mesh*           m_mesh;
    Material*       m_material;

    GameObjectBase* m_gameObject;
};