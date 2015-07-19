#pragma once

#include <QVariant>
#include <vector>
#include "EditorCommands.h"
#include "ToolsideGameComponent.h"
#include "Math\Transform.h"

using std::string;
using std::vector;

class GameObject;
class QMimeData;

class ComponentModelItem
{
public:
    enum ColumnType { NAME_COLUMN, VALUE_COLUMN };

    ComponentModelItem(string name);
    ~ComponentModelItem();

    void                AddChild(ComponentModelItem* child);
    ComponentModelItem* GetChild(int index);
    ComponentModelItem* GetParent();
    int                 GetChildCount();
    int                 GetIndexInParent();

    virtual void        Refresh();
    virtual QVariant    GetData(ColumnType columnType, int role);
    virtual bool        SetData(QVariant value);
    virtual bool        IsEditable();
    virtual bool        DropData(const QMimeData* data);

protected:
    string                      m_name;
    bool                        m_isHeader;

    ComponentModelItem*         m_parent;
    vector<ComponentModelItem*> m_children;
};

class ComponentModelTransformItem : public ComponentModelItem
{
public:
    ComponentModelTransformItem(string name, GameObject* gameObject, TransformVectorType type);

    void        Refresh();
    QVariant    GetData(ColumnType columnType, int role);
    bool        SetData(QVariant value);

private:
    GameObject*             m_gameObject;
    TransformVectorType     m_vectorType;
    Vector3                 m_vector;
};

class ComponentModelMeshItem : public ComponentModelItem
{

};

class ComponentModelScriptItem : public ComponentModelItem
{
public:
    ComponentModelScriptItem(ToolsideGameComponent* component, int paramIndex);

    void        Refresh();
    QVariant    GetData(ColumnType columnType, int role);
    bool        SetData(QVariant value);
    bool        IsEditable();
    bool        DropData(const QMimeData* data);

private:
    string      GetValueString();

    ToolsideGameComponent*              m_component;
    ComponentParameter::ParameterType   m_valueType;
    ComponentValue                      m_value;
    int                                 m_paramIndex;
};