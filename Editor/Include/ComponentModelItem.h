#pragma once

#include <functional>
#include <QVariant>
#include <vector>
#include "EditorCommands.h"
#include "ToolsideGameComponent.h"
#include "Math\Transform.h"

using std::string;
using std::vector;

class Collider;
class MeshInstance;
class RigidBody;
class ToolsideGameObject;
class QMimeData;

typedef uint MenuOptions;

struct GenericParam
{
    GenericParam();
    GenericParam(string name, ComponentParameter::ParameterType type, ComponentValue value, std::function<void(ComponentValue)> callback);

    string                              Name;
    ComponentParameter::ParameterType   Type;
    ComponentValue                      Value;
    std::function<void(ComponentValue)> Callback;
};

class ComponentModelItem
{
public:
    enum ColumnType         { NAME_COLUMN, VALUE_COLUMN };

    enum ContextMenuOption
    {
        CONTEXTMENU_NONE    = 0x0,
        CONTEXTMENU_COPY    = 0x1,
        CONTEXTMENU_DELETE  = 0x2,
    };

    ComponentModelItem(string name);
    ~ComponentModelItem();

    void                AddChild(ComponentModelItem* child);
    ComponentModelItem* GetChild(int index);
    ComponentModelItem* GetParent();
    int                 GetChildCount();
    int                 GetIndexInParent();
    QVariant            GetData(ColumnType columnType, int role);
    void                Clear();

    void                AddGenericParam(string name, ComponentParameter::ParameterType type, ComponentValue value, std::function<void(ComponentValue)> callback);

    virtual void        Refresh();
    virtual QVariant    GetValueData();
    virtual QVariant    GetBackgroundData(ColumnType columnType);
    virtual QVariant    GetTooltip(ColumnType columnType);
    virtual QVariant    GetState(ColumnType columnType);
    virtual bool        SetData(QVariant value);
    virtual bool        IsEditable();
    virtual bool        DropData(const QMimeData* data);
    virtual void        OnDoubleClick(ColumnType columnType);
    virtual MenuOptions GetMenuOptions();
    virtual bool        HandleMenuSelection(ContextMenuOption selection);

protected:
    string                      m_name;
    bool                        m_isHeader;

    ComponentModelItem*         m_parent;
    vector<ComponentModelItem*> m_children;
};

class ComponentModelGenericParamItem : public ComponentModelItem
{
public:
    ComponentModelGenericParamItem(GenericParam);

    QVariant            GetValueData();
    virtual QVariant    GetState(ColumnType columnType);
    bool                SetData(QVariant value);

private:
    GenericParam        m_param;
};

class ComponentModelTransformItem : public ComponentModelItem
{
public:
    ComponentModelTransformItem(ToolsideGameObject* gameObject);
    ComponentModelTransformItem(string name, ToolsideGameObject* gameObject, TransformVectorType type);

    void                Refresh();
    QVariant            GetValueData();
    bool                SetData(QVariant value);
    MenuOptions         GetMenuOptions();
    bool                HandleMenuSelection(ContextMenuOption selection);

private:
    ToolsideGameObject*     m_gameObject;
    TransformVectorType     m_vectorType;
    Vector3                 m_vector;
};

class ComponentModelMeshItem : public ComponentModelItem
{
public:
    ComponentModelMeshItem(MeshInstance* mesh, bool header = true);

    QVariant            GetValueData();
    virtual QVariant    GetTooltip(ColumnType columnType);
    bool                IsEditable();
    bool                DropData(const QMimeData* data);
    MenuOptions         GetMenuOptions();
    bool                HandleMenuSelection(ContextMenuOption selection);

private:
    MeshInstance*       m_mesh;
};

class ComponentModelShaderItem : public ComponentModelItem
{
public:
    ComponentModelShaderItem(Material* material);

    void                Refresh();
    QVariant            GetValueData();
    virtual QVariant    GetTooltip(ColumnType columnType);
    bool                IsEditable();
    bool                DropData(const QMimeData* data);

private:
    Material*           m_material;
};

class ComponentModelColorItem : public ComponentModelItem
{
public:
    ComponentModelColorItem(string name, Material* material, int paramID);

    QVariant            GetValueData();
    QVariant            GetBackgroundData(ColumnType columnType);
    bool                IsEditable();
    bool                DropData(const QMimeData* data);
    void                OnDoubleClick(ColumnType columnType);

private:
    Material*           m_material;
    int                 m_paramID;
};

class ComponentModelTextureItem : public ComponentModelItem
{
public:
    ComponentModelTextureItem(string name, Material* material, int paramID);

    QVariant            GetValueData();
    virtual QVariant    GetTooltip(ColumnType columnType);
    bool                IsEditable();
    bool                DropData(const QMimeData* data);

private:
    Material*           m_material;
    int                 m_paramID;
};

class ComponentModelColliderItem : public ComponentModelItem
{
public:
    ComponentModelColliderItem(Collider* collider, bool header = true);

private:
    Collider*           m_collider;
};

class ComponentModelRigidBodyItem : public ComponentModelItem
{
public:
    ComponentModelRigidBodyItem(RigidBody* rigidBody, bool header = true);

private:
    RigidBody*           m_rigidBody;
};


class ComponentModelScriptItem : public ComponentModelItem
{
public:
    ComponentModelScriptItem(ToolsideGameComponent* component);
    ComponentModelScriptItem(ToolsideGameComponent* component, int paramIndex);

    void                Refresh();
    QVariant            GetValueData();
    QVariant            GetBackgroundData(ColumnType columnType);
    bool                SetData(QVariant value);
    bool                IsEditable();
    bool                DropData(const QMimeData* data);
    void                OnDoubleClick(ColumnType columnType);
    MenuOptions         GetMenuOptions();
    bool                HandleMenuSelection(ContextMenuOption selection);

private:
    bool                IsDropEnabled();
    string              GetValueDataForResourceParam(unsigned int guid);

    ToolsideGameComponent*              m_component;
    ComponentParameter::ParameterType   m_valueType;
    ComponentValue                      m_value;
    int                                 m_paramIndex;
};