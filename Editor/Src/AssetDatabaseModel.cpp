#include "AssetDatabaseModel.h"
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
        if (info->TypeName() == "Shader")     // TODO fixme
        {
            path = "Shader.glsl";
        }
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