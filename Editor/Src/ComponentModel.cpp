#include "ComponentModel.h"
#include "DebugLogger.h"
#include "GameObject.h"
#include "GameObjectReference.h"
#include "HierarchyModel.h"
#include "ToolsideGameComponent.h"

#include <QtWidgets>
#include <string>
#include <EditorCommands.h>
#include "GameObjectMimeData.h"

using std::string;
using namespace EditorCommands;

ComponentModel::ComponentModel(QObject *parent, GameObject* go)
    : m_gameObject(go), QAbstractTableModel(parent)
{
    m_componentList = go->GetToolsideComponentList();

    m_rowCount = 0;
    vector<ToolsideGameComponent*>::iterator iter = m_componentList.begin();
    for (; iter != m_componentList.end(); iter++)
    {
        if (*iter != NULL)
        {
            int size = (*iter)->GetParameterList().size();
            m_rowCount += (1 + size);
            m_accSizes.push_back(m_rowCount);
        }
    }
}


int ComponentModel::rowCount(const QModelIndex &parent) const
{
    return m_rowCount;
}

int ComponentModel::columnCount(const QModelIndex & parent) const
{
    return 2;
}

QVariant ComponentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole && role != Qt::BackgroundRole)
        return QVariant();

    int row = index.row();
    int col = index.column();
    int componentIndex = CalculateComponentIndex(row);
    int paramIndex = CalculateParamIndex(row, componentIndex);

    if (paramIndex == 0)
    {
        // The entry is the component name
        if (role == Qt::BackgroundRole)
        {
            QColor color(150, 150, 150);
            return QBrush(color);
        }
        if (col == 0)
        {
            return QVariant(m_componentList[componentIndex]->GetDisplayName().c_str());
        }
        return QVariant();
    }

    // The entry is a parameter
    ToolsideGameComponent* component = m_componentList[componentIndex];
    ParamList params = component->GetParameterList();
    ParamPair pair = params[paramIndex-1];
    if (col != 0)
    {
        // Value column
        string valueString;
        if (pair.first.Type == ComponentParameter::TYPE_GAMEOBJECT)
        {
            // For game object values, we show the object's name in addition to the guid
            GameObject* go = GameObjectReference::GetGameObject(pair.second.g);
            if (go == NULL)
            {
                valueString = "<MISSING REF>";
            }
            else
            {
                valueString = go->GetName() + " (" + std::to_string(pair.second.g) + ")";
            }
        }
        else
        {
            valueString = pair.second.GetValueString(pair.first.Type);
        }

        return QVariant(valueString.c_str());
    }
    // Name column
    return QVariant(pair.first.Name.c_str());
}

bool ComponentModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole || !IsEditable(index))
        return false;

    // TODO set data using Editor Commands
    int row = index.row();
    int componentIndex = CalculateComponentIndex(row);
    int paramIndex = CalculateParamIndex(row, componentIndex);

    ToolsideGameComponent* component = m_componentList[componentIndex];
    ParamList params = component->GetParameterList();
    ParamPair& pair = params[paramIndex - 1];
    pair.second.SetValue(pair.first.Type, value.toString().toStdString());
    component->SetParameter(pair.first, pair.second);

    return true;
}

Qt::ItemFlags ComponentModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);

    if (index.isValid() && IsEditable(index))
    {
        return  Qt::ItemIsEditable | Qt::ItemIsDropEnabled | defaultFlags;
    }
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

Qt::DropActions ComponentModel::supportedDragActions() const
{
    return Qt::IgnoreAction;
}

// TODO use link action only?
Qt::DropActions ComponentModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction | Qt::LinkAction;
}

QStringList ComponentModel::mimeTypes() const
{
    QStringList types;
    types << "DogwoodEngine/GameObject";
    return types;
}

bool ComponentModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int /*row*/, int /*column*/, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
        return true;

    DebugLogger::Singleton().Log("drop model...");

    if (!parent.isValid())
    {
        return false;
    }

    int row = parent.row();
    int componentIndex = CalculateComponentIndex(row);
    int paramIndex = CalculateParamIndex(row, componentIndex);

    if (paramIndex == 0)
    {
        // The target row is a component name and not a parameter
        return false;
    }

    // The target is a parameter
    ToolsideGameComponent* component = m_componentList[componentIndex];
    ParamList params = component->GetParameterList();
    ParamPair pair = params[paramIndex - 1];
    ComponentParameter::ParameterType paramType = pair.first.Type;
    if (paramType != ComponentParameter::TYPE_GAMEOBJECT)
    {
        // The target parameter is not a gameobject
        return false;
    }

    // The target is a parameter of the correct type, so set its value using the dropped data
    // TODO set data using Editor Commands
    GameObjectMimeData* goData = (GameObjectMimeData*)(data);
    pair.second.SetValue(paramType, std::to_string(goData->getGameObject()->GetID()));
    component->SetParameter(pair.first, pair.second);

    DebugLogger::Singleton().Log("...success!");
    return true;
}

int ComponentModel::CalculateComponentIndex(int row) const
{
    // TODO this is pretty hacky
    int componentIndex = 0;
    while (m_accSizes[componentIndex] <= row)
    {
        componentIndex++;
    }
    return componentIndex;
}

int ComponentModel::CalculateParamIndex(int row, int componentIndex) const
{
    int offset = 0;
    if (componentIndex > 0)
    {
        offset = m_accSizes[componentIndex - 1];
    }
    int paramIndex = row - offset;

    return paramIndex;
}

bool ComponentModel::IsEditable(const QModelIndex &index) const
{
    if (!index.isValid())
        return false;

    int row = index.row();
    int col = index.column();
    int componentIndex = CalculateComponentIndex(row);
    int paramIndex = CalculateParamIndex(row, componentIndex);

    // If the entry is a component name, or a parameter name, it can't be edited
    if (paramIndex == 0 || col == 0)
        return false;

    return true;
}