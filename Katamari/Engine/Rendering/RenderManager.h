#pragma once

class GameObject;

class RenderManager
{
public:
    void RenderScene(GameObject* rootObject);   // TODO should pass in camera & light data
};