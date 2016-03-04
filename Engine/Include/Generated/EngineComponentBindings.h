// !!!!! This file is auto-generated, and any changes will be overwritten when scripts are imported

#pragma once

#include <string>

#include "GameComponentFactory.h"
#include "ToolsideGameComponent.h"
#include "Scene\ResourceManager.h"

// COMPONENT HEADERS
#include "Components/ParticleSystem.h"
#include "Components/TestComponent.h"

using std::string;
using std::unordered_map;

class GameComponent;

// PARAMETER SETTER DECLARATIONS
void SetParameter_GUID_248907459(ParticleSystem* comp, RuntimeParamList* params);
void SetParameter_GUID_4141601931(TestComponent* comp, RuntimeParamList* params);

// CLASS DEFINITION
class EngineFactory : public GameComponentFactory
{
    GameComponent* CreateComponent(unsigned int guid)
    {
        switch (guid)
        {

// COMPONENT CREATION SWTICH
        case 248907459: return new ParticleSystem();
        case 4141601931: return new TestComponent();

        default: printf("Error: missing guid in generated bindings file: %d\n", guid); break;
        }
        return NULL;
    }

    void SetParams(unsigned int guid, GameComponent* component, RuntimeParamList* params)
    {
        switch (guid)
        {

// PARAMETER SETTER SWTICH
        case 248907459: return SetParameter_GUID_248907459((ParticleSystem*)component, params);
        case 4141601931: return SetParameter_GUID_4141601931((TestComponent*)component, params);

        default: printf("Error: missing guid in generated bindings file: %d\n", guid); break;
        }
    }
};

// PARAMETER SETTER DEFINITIONS

void SetParameter_GUID_248907459(ParticleSystem* comp, RuntimeParamList* params)
{
    if (params->size() < 5) return;

    comp->Color = (*params)[0].c;
    comp->MinAge = (*params)[1].f;
    comp->MaxAge = (*params)[2].f;
    comp->MinVelocity = (*params)[3].v;
    comp->MaxVelocity = (*params)[4].v;
}

void SetParameter_GUID_4141601931(TestComponent* comp, RuntimeParamList* params)
{
    if (params->size() < 2) return;

    comp->IntParam = (*params)[0].i;
    comp->BoolParam = (*params)[1].b;
}

