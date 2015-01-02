#pragma once

#include <QAbstractTableModel>

class AssetDatabaseModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    AssetDatabaseModel(QObject *parent);

    int         rowCount(const QModelIndex &parent = QModelIndex()) const;
    int         columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant    data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};