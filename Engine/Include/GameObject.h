#pragma once

//////////////////////////////////////////////////////////////////////////
// GameObject is the in-game / runtime implementation of a game object.
// This class should not be instantiated toolside.
//////////////////////////////////////////////////////////////////////////

#include <float.h>
#include <string>
#include <vector>

#include "GameObjectBase.h"

using std::string;
using std::vector;

class GameComponent;
class MeshInstance;
class ToolsideGameComponent;

class GameObject : public GameObjectBase
{
public:
    GameObject(unsigned int guid, string name = "", GameObjectBase* parent = NULL);
    ~GameObject();

    void    AddComponent(GameComponent* component);

    void    OnCreate();
    void    OnDestroy();
    void    OnStart();
    void    Update(float deltaTime);
    void    OnEnable();
    void    OnDisable();

    void    SetActive(bool active);
    bool    IsActiveSelf();
    bool    IsActiveInHierarchy();

    // TODO this shouldn't be here
    void    Render(Transform& parentWorldTransform, bool dirty, bool wireframe = false);

private:
    bool                    m_active;

    vector<GameComponent*>  m_components;
};