#ifdef GRAPHICS_DX

#include "Rendering/DX/DXMesh.h"

#include "Rendering/DX/DXDebugDraw.h"
#include "Rendering/Image.h"
#include "Rendering/ModelLoading.h"
#include "Rendering/RenderManager.h"

Mesh* Mesh::Create()
{
    return new DXMesh();
}

void Mesh::Destroy(Mesh* mesh)
{
    delete mesh;
}

void DXMesh::Init(std::string path, ResourceInfo* resourceInfo)
{
   
}

void DXMesh::Render(Transform& transform, Material* material, bool wireframe)
{

}

void DXMesh::Delete()
{

}

float DXMesh::GetBoundingRadius()
{
    return 0;
}

int DXMesh::GetTriangleCount()
{
    return 0;
}

void DXMesh::GetTriangle(int index, Vector3* triangle)
{

}

#endif