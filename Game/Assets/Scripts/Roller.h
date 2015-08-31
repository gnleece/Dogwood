#pragma once

#include "GameComponent.h"
#include "GameObjectReference.h"
#include "Math\Algebra.h"

class Mesh;
class Texture;

class Roller : public GameComponent
{
public:

#pragma region Serializable
    float               Speed = 0.5f;
    float               Radius = 0.1f;
    GameObjectReference RotationChild;
    Texture*            MyTex;
    Mesh*               MyMesh;
#pragma endregion

    void Update(float deltaTime);

private:
    Matrix4x4 m_parentMatrix;
    Matrix4x4 m_childMatrix;

    bool m_setupDone = false;
};