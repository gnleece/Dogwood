#pragma once

#include <QAbstractItemModel>
#include "GameObject.h"

class HierarchyModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    HierarchyModel(GameObject* root, QObject *parent = 0);
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
    bool            insertChild(QModelIndex parentIndex, GameObject* parent, GameObject* child, int position);
    bool            removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

    Qt::DropActions supportedDropActions() const;
    QStringList     mimeTypes() const;
    QMimeData*      mimeData(const QModelIndexList &indexes) const;
    bool            dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

private:
    GameObject*   getItem(const QModelIndex &index) const;

    GameObject*   m_rootItem;
};