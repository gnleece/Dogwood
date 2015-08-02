#pragma once

#include <QVariant>
#include <vector>
#include "EditorCommands.h"
#include "ToolsideGameComponent.h"
#include "Math\Transform.h"

using std::string;
using std::vector;

class GameObject;
class MeshInstance;
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
    QVariant            GetData(ColumnType columnType, int role);

    virtual void        Refresh();
    virtual QVariant    GetValueData();
    virtual QVariant    GetBackgroundData(ColumnType columnType);
    virtual bool        SetData(QVariant value);
    virtual bool        IsEditable();
    virtual bool        DropData(const QMimeData* data);
    virtual void        OnDoubleClick();

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

    void                Refresh();
    QVariant            GetValueData();
    bool                SetData(QVariant value);

private:
    GameObject*             m_gameObject;
    TransformVectorType     m_vectorType;
    Vector3                 m_vector;
};

class ComponentModelMeshItem : public ComponentModelItem
{
public:
    ComponentModelMeshItem(string name, MeshInstance* mesh);

    virtual QVariant    GetValueData();
    virtual bool        IsEditable();
    virtual bool        DropData(const QMimeData* data);

private:
    MeshInstance*       m_mesh;
};

class ComponentModelShaderItem : public ComponentModelItem
{
public:
    ComponentModelShaderItem(string name, Material* material);

    virtual QVariant    GetValueData();
    virtual bool        IsEditable();
    virtual bool        DropData(const QMimeData* data);

private:
    Material*           m_material;
};

class ComponentModelColorItem : public ComponentModelItem
{
public:
    ComponentModelColorItem(string name, Material* material, int paramID);

    virtual QVariant    GetValueData();
    QVariant            GetBackgroundData(ColumnType columnType);
    virtual bool        IsEditable();
    virtual bool        DropData(const QMimeData* data);
    void                OnDoubleClick();

private:
    Material*           m_material;
    int                 m_paramID;
};

class ComponentModelTextureItem : public ComponentModelItem
{
public:
    ComponentModelTextureItem(string name, Material* material, int paramID);

    virtual QVariant    GetValueData();
    virtual bool        IsEditable();
    virtual bool        DropData(const QMimeData* data);

private:
    Material*           m_material;
    int                 m_paramID;
};

class ComponentModelScriptItem : public ComponentModelItem
{
public:
    ComponentModelScriptItem(ToolsideGameComponent* component, int paramIndex);

    void                Refresh();
    QVariant            GetValueData();
    QVariant            GetBackgroundData(ColumnType columnType);
    bool                SetData(QVariant value);
    bool                IsEditable();
    bool                DropData(const QMimeData* data);
    void                OnDoubleClick();

private:
    ToolsideGameComponent*              m_component;
    ComponentParameter::ParameterType   m_valueType;
    ComponentValue                      m_value;
    int                                 m_paramIndex;
};