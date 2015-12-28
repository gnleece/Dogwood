// !!!!! This file is auto-generated, and any changes will be overwritten when scripts are imported

#pragma once

#include <string>

#include "GameComponentFactory.h"
#include "ToolsideGameComponent.h"
#include "Scene\ResourceManager.h"

// COMPONENT HEADERS
#include "Roller.h"
#include "Bouncer.h"
#include "Translator.h"
#include "CameraControls.h"
#include "FooComponent.h"
#include "SpinningComponent.h"

using std::string;
using std::unordered_map;

class GameComponent;

// PARAMETER SETTER DECLARATIONS
void SetParameter_GUID_3055461197(Roller* comp, RuntimeParamList* params);
void SetParameter_GUID_3988628104(Bouncer* comp, RuntimeParamList* params);
void SetParameter_GUID_943277816(Translator* comp, RuntimeParamList* params);
void SetParameter_GUID_1661142292(CameraControls* comp, RuntimeParamList* params);
void SetParameter_GUID_1515607268(FooComponent* comp, RuntimeParamList* params);
void SetParameter_GUID_3075022151(SpinningComponent* comp, RuntimeParamList* params);

class MyFactory : public GameComponentFactory
{
    GameComponent* CreateComponent(unsigned int guid)
    {
        switch (guid)
        {

// COMPONENT CREATION SWTICH
        case 3055461197: return new Roller();
        case 3988628104: return new Bouncer();
        case 943277816: return new Translator();
        case 1661142292: return new CameraControls();
        case 1515607268: return new FooComponent();
        case 3075022151: return new SpinningComponent();

        default: printf("Error: missing guid in generated bindings file: %d\n", guid); break;
        }
        return NULL;
    }

    void SetParams(unsigned int guid, GameComponent* component, RuntimeParamList* params)
    {
        switch (guid)
        {

// PARAMETER SETTER SWTICH
        case 3055461197: return SetParameter_GUID_3055461197((Roller*)component, params);
        case 3988628104: return SetParameter_GUID_3988628104((Bouncer*)component, params);
        case 943277816: return SetParameter_GUID_943277816((Translator*)component, params);
        case 1661142292: return SetParameter_GUID_1661142292((CameraControls*)component, params);
        case 1515607268: return SetParameter_GUID_1515607268((FooComponent*)component, params);
        case 3075022151: return SetParameter_GUID_3075022151((SpinningComponent*)component, params);

        default: printf("Error: missing guid in generated bindings file: %d\n", guid); break;
        }
    }
};

// PARAMETER SETTER DEFINITIONS

void SetParameter_GUID_3055461197(Roller* comp, RuntimeParamList* params)
{
    if (params->size() < 3) return;

    comp->Speed = (*params)[0].f;
    comp->Radius = (*params)[1].f;
    comp->CameraObject = (*params)[2].go;
}

void SetParameter_GUID_3988628104(Bouncer* comp, RuntimeParamList* params)
{
    if (params->size() < 2) return;

    comp->BounceHeight = (*params)[0].f;
    comp->BounceSpeed = (*params)[1].f;
}

void SetParameter_GUID_943277816(Translator* comp, RuntimeParamList* params)
{
    if (params->size() < 1) return;

    comp->Speed = (*params)[0].f;
}

void SetParameter_GUID_1661142292(CameraControls* comp, RuntimeParamList* params)
{
    if (params->size() < 6) return;

    comp->TargetObject = (*params)[0].go;
    comp->CameraObject = (*params)[1].go;
    comp->Distance = (*params)[2].f;
    comp->HorizSpeed = (*params)[3].f;
    comp->VertSpeed = (*params)[4].f;
    comp->VertMax = (*params)[5].f;
}

void SetParameter_GUID_1515607268(FooComponent* comp, RuntimeParamList* params)
{
    if (params->size() < 2) return;

    comp->BarParam = (*params)[0].f;
    comp->IntParam = (*params)[1].i;
}

void SetParameter_GUID_3075022151(SpinningComponent* comp, RuntimeParamList* params)
{
    if (params->size() < 6) return;

    comp->SpinType = (*params)[0].i;
    comp->Speed = (*params)[1].f;
    comp->EnableSpinning = (*params)[2].b;
    comp->SpinLabel = (*params)[3].str;
    comp->Offset = (*params)[4].v;
    comp->SpinColor = (*params)[5].c;
}

