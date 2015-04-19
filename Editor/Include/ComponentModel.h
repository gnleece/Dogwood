#pragma once

#include <QAbstractItemModel>
#include <vector>

class ComponentModelItem;
class GameObject;
class ResourceInfo;

using std::vector;

class ComponentModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    ComponentModel(QObject *parent, GameObject* go);

    QModelIndex     index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex     parent(const QModelIndex &index) const;

    int             rowCount(const QModelIndex &parent = QModelIndex()) const;
    int             columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant        data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags   flags(const QModelIndex &index) const;

    bool            insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
    bool            removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

    ComponentModelItem*   getItem(const QModelIndex &index) const;

private:
    GameObject*         m_gameObject;
    ComponentModelItem* m_itemRoot;
};