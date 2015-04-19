#include "ComponentModel.h"
#include "ComponentModelItem.h"
#include "GameObject.h"
#include "HierarchyModel.h"

#include <QtWidgets>
#include <string>
#include <EditorCommands.h>
#include "GameObjectMimeData.h"

using std::string;
using namespace EditorCommands;

ComponentModel::ComponentModel(QObject *parent, GameObject* go)
    : m_gameObject(go), QAbstractItemModel(parent)
{
}

QModelIndex ComponentModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    ComponentModelItem* parentItem = getItem(parent);
    ComponentModelItem* childItem = parentItem->GetChild(row);

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex ComponentModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    ComponentModelItem *childItem = getItem(index);
    ComponentModelItem *parentItem = childItem->GetParent();

    if (parentItem == m_itemRoot)
        return QModelIndex();

    return createIndex(parentItem->GetChildNumber(), 0, parentItem);
}

int ComponentModel::rowCount(const QModelIndex &parent) const
{
    ComponentModelItem *parentItem = getItem(parent);
    return parentItem->GetChildCount();
}

int ComponentModel::columnCount(const QModelIndex & /* parent */) const
{
    return 1;//2;
}

QVariant ComponentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    ComponentModelItem *item = getItem(index);

    return QVariant(item->GetName().c_str());
}

Qt::ItemFlags ComponentModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
    return defaultFlags;

    /*
    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | defaultFlags;
    else
        return defaultFlags;
    */
}


bool ComponentModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    ComponentModelItem *parentItem = getItem(parent);

    beginInsertRows(parent, position, position + rows - 1);
    bool success = parentItem->InsertChildren(position, rows);
    endInsertRows();

    return success;
}

bool ComponentModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    ComponentModelItem *parentItem = getItem(parent);

    beginRemoveRows(parent, position, position + rows - 1);
    bool success = parentItem->RemoveChildren(position, rows);
    endRemoveRows();

    return success;
}

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