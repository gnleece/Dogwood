#include "ComponentModel.h"
#include "GameObject.h"
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
            int size = (*iter)->GetParameterList()->size();
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

    // TODO this is pretty hacky
    int componentIndex = 0;
    while(m_accSizes[componentIndex] <= row)
    {
        componentIndex++;
    }

    int offset = 0;
    if (componentIndex > 0)
    {
        offset = m_accSizes[componentIndex - 1];
    }
    int paramIndex = row - offset;

    // TODO testing, remove
    if (paramIndex == 0)
    {
        if (role == Qt::BackgroundRole)
        {
            QColor color(150, 150, 150);
            return QBrush(color);
        }
        if (col == 0)
        {
            return QVariant(m_componentList[componentIndex]->GetGuid());    // TODO use script name instead of guid
        }
        return QVariant();
    }
    return QVariant("Parameter");
}
