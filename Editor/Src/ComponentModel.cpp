#include "ComponentModel.h"
#include "ComponentModelItem.h"
#include "DebugLogger.h"
#include "GameObject.h"
#include "GameObjectReference.h"
#include "ToolsideGameComponent.h"

#include <QtWidgets>
#include <string>
#include <EditorCommands.h>

using std::string;
using namespace EditorCommands;

ComponentModel::ComponentModel(QObject *parent, GameObject* go)
    : m_gameObject(go), QAbstractItemModel(parent)
{
    BuildModel();
}

void ComponentModel::BuildModel()
{
    m_rootItem = new ComponentModelItem("ROOT");

    AddTransformData();
    AddMeshData();
    AddComponentData();

    emit layoutChanged();
}

void ComponentModel::RefreshModel()
{
    ClearModel();
    BuildModel();

    emit layoutChanged();
}

void ComponentModel::ClearModel()
{
    delete m_rootItem;      // The item destructor will take care of deleting child items recursively
}

ComponentModelItem* ComponentModel::GetItem(const QModelIndex &index) const
{
    if (index.isValid())
    {
        ComponentModelItem *item = static_cast<ComponentModelItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return m_rootItem;
}

QModelIndex ComponentModel::index(int row, int column, const QModelIndex& parent) const
{
    ComponentModelItem* parentItem = GetItem(parent);
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

    ComponentModelItem* childItem = GetItem(index);
    ComponentModelItem* parentItem = childItem->GetParent();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->GetIndexInParent(), 0, parentItem);
}

int ComponentModel::rowCount(const QModelIndex& index) const
{
    ComponentModelItem* item = GetItem(index);
    return item->GetChildCount();
}

int ComponentModel::columnCount(const QModelIndex& /*index*/) const
{
    return 2;
}

QVariant ComponentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole && role != Qt::BackgroundRole && role != Qt::TextAlignmentRole)
        return QVariant();

    int col = index.column();
    ComponentModelItem *item = GetItem(index);
    return item->GetData((ComponentModelItem::ColumnType)col, role);
}

bool ComponentModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (role != Qt::EditRole || !IsEditable(index))
        return false;

    ComponentModelItem* item = GetItem(index);
    return item->SetData(value);
}

Qt::ItemFlags ComponentModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    if (index.isValid() && IsEditable(index))
    {
        return  Qt::ItemIsEditable | Qt::ItemIsDropEnabled | defaultFlags;
    }
    else
    {
        return Qt::ItemIsDropEnabled | defaultFlags;
    }
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
    types << "DogwoodEngine/GameObject" << "DogwoodEngine/AssetInfo";
    return types;
}

bool ComponentModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int /*row*/, int /*column*/, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
        return true;

    if (!parent.isValid())
        return false;

    ComponentModelItem* item = GetItem(parent);
    bool success = item->DropData(data);

    ClearModel();
    BuildModel();

    return success;
}

bool ComponentModel::IsEditable(const QModelIndex &index) const
{
    if (!index.isValid())
        return false;

    int col = index.column();
    ComponentModelItem *item = GetItem(index);
    if (col == 0 || !item->IsEditable())
        return false;

    return true;
}

void ComponentModel::AddTransformData()
{
    ComponentModelTransformItem* transformItem = new ComponentModelTransformItem(m_gameObject);
    m_rootItem->AddChild(transformItem);
}

void ComponentModel::AddMeshData()
{
    MeshInstance* meshInstance = m_gameObject->GetMesh();
    if (meshInstance == NULL)
        return;

    ComponentModelMeshItem* meshItem = new ComponentModelMeshItem(meshInstance);
    m_rootItem->AddChild(meshItem);
}

void ComponentModel::AddComponentData()
{
    m_componentList = m_gameObject->GetToolsideComponentList();

    // Add data for each script component on the game object
    vector<ToolsideGameComponent*>::iterator iter = m_componentList.begin();
    for (; iter != m_componentList.end(); iter++)
    {
        // Add header for this script
        ToolsideGameComponent* component = *iter;
        ComponentModelScriptItem* headerItem = new ComponentModelScriptItem(component);
        m_rootItem->AddChild(headerItem);
    }
}