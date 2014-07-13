#include "HierarchyModel.h"
#include "HierarchyItem.h"

#include <QtWidgets>
#include <string>

using std::string;

HierarchyModel::HierarchyModel(QObject *parent)
: QAbstractItemModel(parent)
{
    string rootData = "Game Object Hierarchy";

    m_rootItem = new HierarchyItem(rootData);
    modelSetup(m_rootItem);
}

HierarchyModel::~HierarchyModel()
{
    delete m_rootItem;
}

QModelIndex HierarchyModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    HierarchyItem *parentItem = getItem(parent);

    HierarchyItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex HierarchyModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    HierarchyItem *childItem = getItem(index);
    HierarchyItem *parentItem = childItem->parent();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

QVariant HierarchyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    HierarchyItem *item = getItem(index);
    return QVariant(item->getData().c_str());
}

QVariant HierarchyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(m_rootItem->getData().c_str());

    return QVariant();
}

bool HierarchyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    HierarchyItem *item = getItem(index);
    bool result = item->setData(value.toString().toStdString());

    if (result)
        emit dataChanged(index, index);

    return result;
}

Qt::ItemFlags HierarchyModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

int HierarchyModel::columnCount(const QModelIndex & /* parent */) const
{
    return 1;
}

int HierarchyModel::rowCount(const QModelIndex &parent) const
{
    HierarchyItem *parentItem = getItem(parent);
    return parentItem->childCount();
}

bool HierarchyModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    HierarchyItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows);
    endInsertRows();

    return success;
}

bool HierarchyModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    HierarchyItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

HierarchyItem *HierarchyModel::getItem(const QModelIndex &index) const
{
    if (index.isValid())
    {
        HierarchyItem *item = static_cast<HierarchyItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return m_rootItem;
}

void HierarchyModel::modelSetup(HierarchyItem *parent)
{
    // TEST DATA SETUP
    for (int i = 0; i < 3; i++)
    {
        parent->insertChildren(parent->childCount(), 1);
        parent->child(parent->childCount() - 1)->setData("child");
        HierarchyItem* next = parent->child(parent->childCount() - 1);
        next->insertChildren(next->childCount(), 1);
        next->child(next->childCount() - 1)->setData("sub child");
    }
}