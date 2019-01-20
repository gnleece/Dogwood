#pragma once

#include "Math\Transform.h"
#include "Scene\Resource.h"
#include "ShaderProgram.h"

#include <vector>

class Material;
class ResourceInfo;

class Mesh : public Resource
{
public:
    static Mesh* Create();
    static void Destroy(Mesh* mesh);

    virtual ~Mesh() {}

    virtual void    Init(std::string filename, ResourceInfo* resourceInfo) = 0;

    virtual void    Render(Transform& transform, Material* material, bool wireframe = false) = 0;
    virtual void    Delete() = 0;

    virtual float   GetBoundingRadius() = 0;

    virtual int     GetTriangleCount() = 0;
    virtual void    GetTriangle(int index, Vector3* triangle) = 0;
};