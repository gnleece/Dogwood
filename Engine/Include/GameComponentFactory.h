#pragma once

#include "ToolsideGameComponent.h"

class GameComponent;

class GameComponentFactory
{
public:
    virtual GameComponent* CreateComponent(unsigned int guid) = 0;
    virtual void SetParams(unsigned int guid, GameComponent* component, RuntimeParamList* params) = 0;
};