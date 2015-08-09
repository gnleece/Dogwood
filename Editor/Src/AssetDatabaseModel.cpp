#include "AssetDatabaseModel.h"
#include "AssetMimeData.h"
#include "Scene\ResourceManager.h"

#include <unordered_map>
#include <qfileinfo.h>

AssetDatabaseModel::AssetDatabaseModel(QObject *parent)
: QAbstractTableModel(parent)
{
    ResourceMap map = ResourceManager::Singleton().GetResourceMap();
    ResourceMap::iterator iter = map.begin();
    for (; iter != map.end(); iter++)
    {
        m_infos.push_back(iter->second);        // TODO update on asset import
    }
}

int AssetDatabaseModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_infos.size();
}

int AssetDatabaseModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 2;
}

QVariant AssetDatabaseModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    int col = index.column();

    ResourceInfo* info = m_infos[row];
    if (info == NULL)
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        string path = info->path;
        if (col == 0)
        {
            QFileInfo fileInfo(path.c_str());
            return fileInfo.baseName();
        }
        else
        {
            return QString(path.c_str());
        }
    }
    else if (role == FilterRole)
    {
        return QString(info->TypeName().c_str());
    }

    return QVariant();
}

ResourceInfo* AssetDatabaseModel::getItem(const QModelIndex &index) const
{
    if (index.isValid())
    {
        int row = index.row();

        ResourceInfo *item = m_infos[row];
        if (item)
            return item;
    }
    return NULL;
}

Qt::ItemFlags AssetDatabaseModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);
    return Qt::ItemIsDragEnabled | defaultFlags;
}

Qt::DropActions AssetDatabaseModel::supportedDragActions() const
{
    return Qt::LinkAction;
}

Qt::DropActions AssetDatabaseModel::supportedDropActions() const
{
    return Qt::LinkAction;
}

QStringList AssetDatabaseModel::mimeTypes() const
{
    QStringList types;
    types << "DogwoodEngine/AssetInfo";
    return types;
}

QMimeData* AssetDatabaseModel::mimeData(const QModelIndexList &indexes) const
{
    AssetMimeData* mimeData;
    foreach(const QModelIndex &index, indexes)
    {
        if (index.isValid())
        {
            mimeData = new AssetMimeData(getItem(index));
            break;
        }
    }

    return mimeData;
}