#include "RenderManager.h"

#include "..\GameObject.h"

void RenderManager::RenderScene(GameObject* rootObject)
{
    rootObject->Render(Matrix4x4::Identity, false);
}