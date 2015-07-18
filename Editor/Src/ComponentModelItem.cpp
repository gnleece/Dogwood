#include "ComponentModelItem.h"
#include "DebugLogger.h"
#include "GameObject.h"
#include "GameObjectMimeData.h"
#include "GameObjectReference.h"
#include <QBrush>
#include <QColor>

ComponentModelItem::ComponentModelItem(string name)
    : m_name(name), m_isHeader(true), m_parent(NULL)
{}

ComponentModelItem::~ComponentModelItem()
{
    while(m_children.size() > 0)
    {
        ComponentModelItem* child = m_children.back();
        m_children.pop_back();
        delete child;
    }
    m_children.clear();
}

void ComponentModelItem::AddChild(ComponentModelItem* child)
{
    if (child == NULL)
        return;

    m_children.push_back(child);
    child->m_parent = this;
}

ComponentModelItem* ComponentModelItem::GetChild(int index)
{
    if (index < 0 || index >= m_children.size())
        return NULL;

    return m_children[index];
}

ComponentModelItem* ComponentModelItem::GetParent()
{
    return m_parent;
}

int ComponentModelItem::GetChildCount()
{
    return m_children.size();
}

int ComponentModelItem::GetIndexInParent()
{
    // determine the index of this game object in the parent's list of children
    if (m_parent)
    {
        vector<ComponentModelItem*>::iterator iter = std::find(m_parent->m_children.begin(), m_parent->m_children.end(), const_cast<ComponentModelItem*>(this));
        return std::distance(m_parent->m_children.begin(), iter);
    }

    return 0;
}

QVariant ComponentModelItem::GetData(ColumnType columnType, int role)
{
    if (role == Qt::BackgroundRole)
    {
        // TODO move this color stuff into the view
        if (m_isHeader)
        {
            QColor color(230, 230, 230);
            return QBrush(color);
        }
    }
    else
    {
        switch (columnType)
        {
        case NAME_COLUMN:
            return QVariant(m_name.c_str());
        }
    }
}

bool ComponentModelItem::SetData(QVariant value)
{
    return false;
}

bool ComponentModelItem::IsEditable()
{
    return !m_isHeader;
}

bool ComponentModelItem::DropData(const QMimeData* data)
{
    return false;
}

//--------------------------------------------------------------------------------

ComponentModelScriptItem::ComponentModelScriptItem(ToolsideGameComponent* component, int paramIndex)
    : ComponentModelItem(""), m_component(component)
{
    ParamList params = m_component->GetParameterList();
    ParamPair& pair = params[paramIndex];

    m_name = pair.first.Name;
    m_valueType = pair.first.Type;
    m_value = pair.second;

    m_isHeader = false;
}

QVariant ComponentModelScriptItem::GetData(ColumnType columnType, int role)
{
    if (role == Qt::BackgroundRole)
    {
        // TODO move this color stuff into the view
        if (m_isHeader)
        {
            QColor color(230, 230, 230);
            return QBrush(color);
        }
    }
    else
    {
        switch (columnType)
        {
        case NAME_COLUMN:
            return QVariant(m_name.c_str());
        case VALUE_COLUMN:
            string str = GetValueString();
            return QVariant(str.c_str());
            break;
        }
    }
}

string ComponentModelScriptItem::GetValueString()
{
    string valueString;

    if (m_valueType == ComponentParameter::TYPE_GAMEOBJECT)
    {
        // For game object values, we show the object's name in addition to the guid
        GameObject* go = GameObjectReference::GetGameObject(m_value.g);
        if (go == NULL)
        {
            valueString = "<MISSING REF>";
        }
        else
        {
            valueString = go->GetName() + " (" + std::to_string(m_value.g) + ")";
        }
    }
    else
    {
        valueString = m_value.GetValueString(m_valueType);
    }

    return valueString;
}

bool ComponentModelScriptItem::SetData(QVariant value)
{
    // TODO set data using Editor Commands
    m_value.SetValue(m_valueType, value.toString().toStdString());
    m_component->SetParameter(ComponentParameter(m_name, m_valueType), m_value);

    return false;
}

bool ComponentModelScriptItem::IsEditable()
{
    if (m_isHeader)
        return false;

    if (m_valueType == ComponentParameter::TYPE_GAMEOBJECT)
        return false;       // reference types can't be edited directly

    return true;
}

bool ComponentModelScriptItem::DropData(const QMimeData* data)
{
    if (m_isHeader)
        return false;

    if (m_valueType != ComponentParameter::TYPE_GAMEOBJECT)
        return false;       // only reference types support drag & drop

    // TODO set data using Editor Commands
    GameObjectMimeData* goData = (GameObjectMimeData*)(data);
    m_value.SetValue(m_valueType, std::to_string(goData->getGameObject()->GetID()));
    m_component->SetParameter(ComponentParameter(m_name, m_valueType), m_value);

    return true;
}