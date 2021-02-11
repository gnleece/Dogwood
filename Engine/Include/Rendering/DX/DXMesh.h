#pragma once

#ifdef GRAPHICS_DX

#include "Rendering/Mesh.h"

class DXMesh : public Mesh
{
public:
    friend class Mesh;

    virtual void    Init(std::string filename, ResourceInfo* resourceInfo);

    virtual void    Render(Transform& transform, Material* material, bool wireframe = false);
    virtual void    Delete();

    virtual float   GetBoundingRadius();

    virtual int     GetTriangleCount();
    virtual void    GetTriangle(int index, Vector3* triangle);
};

#endif