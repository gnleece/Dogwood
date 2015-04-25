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
    vector<ToolsideGameComponent* > componentList = go->GetToolsideComponentList();
}


int ComponentModel::rowCount(const QModelIndex &parent) const
{
    return 6;
}

int ComponentModel::columnCount(const QModelIndex & parent) const
{
    return 2;
}

QVariant ComponentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    //ComponentModelItem *item = getItem(index);

    return QVariant("Parameter");
    //return QVariant(item->GetName().c_str());
}
/*
ComponentModelItem* ComponentModel::getItem(const QModelIndex &index) const
{
    if (index.isValid())
    {
        ComponentModelItem *item = static_cast<ComponentModelItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return m_itemRoot;
}
*/