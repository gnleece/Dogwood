#include "HierarchyModel.h"

#include <QtWidgets>
#include <string>
#include <EditorCommands.h>

#include "DebugLogger.h"
#include "GameObjectMimeData.h"
#include "ToolsideGameObject.h"

using std::string;
using namespace EditorCommands;

HierarchyModel::HierarchyModel(ToolsideGameObject* root, QObject *parent)
: m_rootItem(root),
  QAbstractItemModel(parent)
{
    if (root == NULL)
    {
        printf("Error creating HierarchyModel. Root object is null.\n");
        return;
    }

    m_rootItem = root;
}

HierarchyModel::~HierarchyModel()
{
}

QModelIndex HierarchyModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    ToolsideGameObject* parentItem = getItem(parent);

    ToolsideGameObject* childItem = parentItem->GetChild(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex HierarchyModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    ToolsideGameObject* childItem = getItem(index);
    ToolsideGameObject* parentItem = (ToolsideGameObject*)childItem->GetParent();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->GetChildNumber(), 0, parentItem);
}

QVariant HierarchyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole && role != MatchRole)
        return QVariant();

    ToolsideGameObject *item = getItem(index);

    if (role == MatchRole)
    {
        return QVariant(item->GetID());
    }

    return QVariant(item->GetName().c_str());
}

// This function is an implementation of a virtual function provided by QAbstractItemModel.
// It is called automatically when the user edits the name of an object in the editor window.
// Instead of actually setting the name here, we use a Command so that this action can be
// undone/recond by the CommandManager. The Command will do the actual rename using setItemName()
bool HierarchyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    RenameGameObjectCommand* command = new RenameGameObjectCommand(this, index, value.toString().toStdString());
    CommandManager::Singleton().ExecuteCommand(command);

    return true;
}

// This is the function that ACTUALLY sets the name of an object. It is called by the Command
// for renaming game objects.
void HierarchyModel::setItemName(const QModelIndex &index, string name)
{
    ToolsideGameObject *item = getItem(index);
    item->SetName(name);
    emit dataChanged(index, index);
}

Qt::ItemFlags HierarchyModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

int HierarchyModel::columnCount(const QModelIndex & /* index */) const
{
    return 1;
}

int HierarchyModel::rowCount(const QModelIndex& index) const
{
    ToolsideGameObject *item = getItem(index);
    return item->GetChildCount();
}

bool HierarchyModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    ToolsideGameObject *parentItem = getItem(parent);

    beginInsertRows(parent, position, position + rows - 1);
    bool success = parentItem->InsertChildren(position, rows);
    endInsertRows();

    return success;
}

bool HierarchyModel::insertChild(QModelIndex parentIndex, ToolsideGameObject* parent, ToolsideGameObject* child, int position)
{
    beginInsertRows(parentIndex, position, position);
    bool success = parent->InsertChild(position, child);
    endInsertRows();

    return success;
}

bool HierarchyModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    ToolsideGameObject *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->RemoveChildren(position, rows);
    endRemoveRows();

    return success;
}

ToolsideGameObject* HierarchyModel::getItem(const QModelIndex &index) const
{
    if (index.isValid())
    {
        ToolsideGameObject *item = static_cast<ToolsideGameObject*>(index.internalPointer());
        if (item)
            return item;
    }
    return m_rootItem;
}

// We do a CopyAction instead of a MoveAction for drag & drop so that
// removeRows() doesn't get called automatically by Qt. We want to manually do the
// remove ourselves so that we can make it work with the undo/redo stack
Qt::DropActions HierarchyModel::supportedDragActions() const
{
    return Qt::CopyAction;
}

Qt::DropActions HierarchyModel::supportedDropActions() const
{
    return Qt::CopyAction;
}

QStringList HierarchyModel::mimeTypes() const
{
    QStringList types;
    types << "DogwoodEngine/GameObject";
    return types;
}

QMimeData* HierarchyModel::mimeData(const QModelIndexList &indexes) const
{
    // TODO support list of game objects
    GameObjectMimeData *mimeData;
    foreach(const QModelIndex &index, indexes)
    {
        if (index.isValid())
        {
            mimeData = new GameObjectMimeData(getItem(index), index.row(), index.parent());
            break;      // only grab first object TODO fix this
        }
    }

    return mimeData;
}

bool HierarchyModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int /*row*/, int column, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
        return true;

    if (!data->hasFormat("DogwoodEngine/GameObject"))
        return false;

    if (column > 0)
        return false;

    GameObjectMimeData* goData = (GameObjectMimeData*)(data);
    ReparentGameObjectCommand* command = new ReparentGameObjectCommand(this, goData, parent);
    CommandManager::Singleton().ExecuteCommand(command);

    return true;
}