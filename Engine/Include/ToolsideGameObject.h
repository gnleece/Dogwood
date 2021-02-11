#pragma once

//////////////////////////////////////////////////////////////////////////
// ToolsideGameObject is used to represent game objects in Editor only
// and should not be used at runtime (in game).
//////////////////////////////////////////////////////////////////////////

#include "GameObjectBase.h"

class ToolsideGameComponent;

class ToolsideGameObject : public GameObjectBase
{
public:
    ToolsideGameObject(unsigned int guid, string name = "", GameObjectBase* parent = NULL);
    ~ToolsideGameObject();

    ToolsideGameObject*     GetChild(int index);
    int                     GetChildNumber();
    int                     GetChildCount();

    bool                    InsertChildren(int position, int count);
    bool                    InsertChild(int position, ToolsideGameObject* child);
    bool                    RemoveChildren(int position, int count);

    void                    AddComponent(ToolsideGameComponent* component);
    void                    RemoveComponent(ToolsideGameComponent* component);

    vector<ToolsideGameComponent*>&  GetComponentList();
    
    ToolsideGameObject*     DeepCopy(ToolsideGameObject* parent = NULL);

    // TODO this shouldn't be here
    void                    Render(bool wireframe = false);
    void                    SetSelected(bool selected);

private:
    vector<ToolsideGameComponent*>  m_components;

    bool                            m_selected;
};