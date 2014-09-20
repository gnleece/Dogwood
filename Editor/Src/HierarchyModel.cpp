#include "HierarchyModel.h"

#include <QtWidgets>
#include <string>

using std::string;

HierarchyModel::HierarchyModel(GameObject* root, QObject *parent)
: m_rootItem(root),
  QAbstractItemModel(parent)
{
    m_rootItem = new GameObject("Game Object Hierarchy");
    root->SetParent(m_rootItem);
}

HierarchyModel::~HierarchyModel()
{
    delete m_rootItem;
}

QModelIndex HierarchyModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    GameObject* parentItem = getItem(parent);

    GameObject* childItem = parentItem->GetChild(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex HierarchyModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    GameObject *childItem = getItem(index);
    GameObject *parentItem = childItem->GetParent();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->GetChildNumber(), 0, parentItem);
}

QVariant HierarchyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    GameObject *item = getItem(index);
    return QVariant(item->GetName().c_str());
}

QVariant HierarchyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(m_rootItem->GetName().c_str());

    return QVariant();
}

bool HierarchyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    GameObject *item = getItem(index);
    item->SetName(value.toString().toStdString());

    emit dataChanged(index, index);     // TODO do we need to test a return value here?

    return true;
}

Qt::ItemFlags HierarchyModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

int HierarchyModel::columnCount(const QModelIndex & /* parent */) const
{
    return 1;
}

int HierarchyModel::rowCount(const QModelIndex &parent) const
{
    GameObject *parentItem = getItem(parent);
    return parentItem->GetChildCount();
}

bool HierarchyModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    GameObject *parentItem = getItem(parent);

    beginInsertRows(parent, position, position + rows - 1);
    bool success = parentItem->InsertChildren(position, rows);
    endInsertRows();

    return success;
}

bool HierarchyModel::insertChild(QModelIndex parentIndex, GameObject* parent, GameObject* child, int position)
{
    beginInsertRows(parentIndex, position, position);
    bool success = parent->InsertChild(position, child);
    endInsertRows();

    return success;
}

bool HierarchyModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    GameObject *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->RemoveChildren(position, rows);
    endRemoveRows();

    return success;
}

GameObject* HierarchyModel::getItem(const QModelIndex &index) const
{
    if (index.isValid())
    {
        GameObject *item = static_cast<GameObject*>(index.internalPointer());
        if (item)
            return item;
    }
    return m_rootItem;
}

Qt::DropActions HierarchyModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList HierarchyModel::mimeTypes() const
{
    QStringList types;
    types << "application/vnd.text.list";
    return types;
}

QMimeData* HierarchyModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach(const QModelIndex &index, indexes)
    {
        if (index.isValid())
        {
            QString text = data(index, Qt::DisplayRole).toString();
            stream << text;
        }
    }

    mimeData->setData("application/vnd.text.list", encodedData);
    return mimeData;
}

bool HierarchyModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
        return true;

    if (!data->hasFormat("application/vnd.text.list"))
        return false;

    if (column > 0)
        return false;

    int beginRow;

    if (row != -1)
        beginRow = row;

    else if (parent.isValid())
        beginRow = parent.row();

    else
        beginRow = rowCount(QModelIndex());

    QByteArray encodedData = data->data("application/vnd.text.list");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QStringList newItems;
    int rows = 0;

    while (!stream.atEnd())
    {
        QString text;
        stream >> text;
        newItems << text;
        ++rows;
    }

    insertRows(beginRow, rows, QModelIndex());
    foreach(const QString &text, newItems)
    {
        QModelIndex idx = index(beginRow, 0, QModelIndex());
        setData(idx, text);
        beginRow++;
    }

    return true;
}