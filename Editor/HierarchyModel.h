#pragma once

#include <QAbstractItemModel>
#include "HierarchyItem.h"

class HierarchyModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    HierarchyModel(QObject *parent = 0);
    ~HierarchyModel();

    QModelIndex     index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex     parent(const QModelIndex &index) const;

    QVariant        data(const QModelIndex &index, int role) const;
    bool            setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant        headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    Qt::ItemFlags   flags(const QModelIndex &index) const;

    int             columnCount(const QModelIndex &parent = QModelIndex()) const;
    
    int             rowCount(const QModelIndex &parent = QModelIndex()) const;
    bool            insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
    bool            removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

private:
    HierarchyItem*   getItem(const QModelIndex &index) const;
    void             modelSetup(HierarchyItem *parent);

    HierarchyItem*   m_rootItem;
};