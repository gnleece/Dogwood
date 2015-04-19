#pragma once

#include <vector>

using std::vector;

class ComponentModelItem
{
public:
    ComponentModelItem* GetParent();
    void                SetParent(ComponentModelItem* parent, int position);

    ComponentModelItem* GetChild(int position);
    int                 GetChildNumber();
    int                 GetChildCount();
    bool                InsertChildren(int position, int count);
    bool                RemoveChildren(int position, int count);

    string              GetName();
    string              GetValue();
    void                SetValue();

private:
    void                AddChild(ComponentModelItem* child, int position = -1);
    void                RemoveChild(ComponentModelItem* child);

    ComponentModelItem* m_parent;

    vector<ComponentModelItem*> m_children;
};