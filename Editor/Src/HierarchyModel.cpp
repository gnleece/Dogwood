#include "HierarchyModel.h"

#include <QtWidgets>
#include <string>
#include <EditorCommands.h>
#include "GameObjectMimeData.h"

using std::string;
using namespace EditorCommands;

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
    GameObject *item = getItem(index);
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
    return Qt::MoveAction;
}

QStringList HierarchyModel::mimeTypes() const
{
    QStringList types;
    types << "DogwoodEngine/GameObject";
    return types;
}

QMimeData* HierarchyModel::mimeData(const QModelIndexList &indexes) const
{
    // TODO support list of gameobjects
    GameObjectMimeData *mimeData;
    foreach(const QModelIndex &index, indexes)
    {
        if (index.isValid())
        {
            GameObject* go = getItem(index);
            mimeData = new GameObjectMimeData(go);
            break;      // only grab first object TODO fix this
        }
    }

    return mimeData;
}

bool HierarchyModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
        return true;

    if (!data->hasFormat("DogwoodEngine/GameObject"))
        return false;

    if (column > 0)
        return false;

    GameObjectMimeData* goData = (GameObjectMimeData*)(data);

    // We copy the game object and insert the new version instead of just using the original,
    // because the original will be deleted by an automatic call to removeRows
    GameObject* go = goData->getGameObject()->DeepCopy();
    if (parent.isValid())
    {
        insertChild(parent, getItem(parent), go, 0);
    }
    else
    {
        // TODO handle dropping at top level (root)
    }

    return true;
}