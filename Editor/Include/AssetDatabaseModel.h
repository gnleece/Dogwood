#pragma once

#include <QAbstractTableModel>
#include <vector>

class ResourceInfo;

using std::vector;

class AssetDatabaseModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    AssetDatabaseModel(QObject *parent);

    int             rowCount(const QModelIndex &parent = QModelIndex()) const;
    int             columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant        data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    ResourceInfo*   getItem(const QModelIndex &index) const;

    static const int FilterRole = Qt::UserRole + 2;

private:
    vector<ResourceInfo*>   m_infos;
};