#include "AssetDatabaseModel.h"
#include "Scene\ResourceManager.h"

#include <unordered_map>

AssetDatabaseModel::AssetDatabaseModel(QObject *parent)
:QAbstractTableModel(parent)
{
}

int AssetDatabaseModel::rowCount(const QModelIndex & /*parent*/) const
{
    return 0;
}

int AssetDatabaseModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 2;
}

QVariant AssetDatabaseModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return QString("Asset path goes here");
    }
    return QVariant();
}