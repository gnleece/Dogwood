// !!!!! This file is auto-generated, and any changes will be overwritten when scripts are imported

#pragma once

#include <string>

#include "GameComponentFactory.h"
#include "ToolsideGameComponent.h"
#include "Scene\ResourceManager.h"

///COMPONENT_HEADERS

using std::string;
using std::unordered_map;

class GameComponent;

///PARAMETER_SETTER_DECLARATIONS

class MyFactory : public GameComponentFactory
{
    GameComponent* CreateComponent(unsigned int guid)
    {
        switch (guid)
        {
///COMPONENT_CREATION_SWITCH

        default: printf("Error: missing guid in generated bindings file: %d\n", guid); break;
        }
        return NULL;
    }

    void SetParams(unsigned int guid, GameComponent* component, RuntimeParamList* params)
    {
        switch (guid)
        {
///PARAMETER_SETTER_SWITCH

        default: printf("Error: missing guid in generated bindings file: %d\n", guid); break;
        }
    }
};

///PARAMETER_SETTER_DEFINITIONS