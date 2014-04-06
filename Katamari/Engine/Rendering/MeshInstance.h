#pragma once

#include "..\GameComponent.h"
#include "Mesh.h"

class MeshInstance : GameComponent
{
public:
    void Render();

private:
    Mesh m_mesh;    // TODO should be shared through ResourceManager
};