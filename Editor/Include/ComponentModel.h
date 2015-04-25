#pragma once

#include <QAbstractTableModel>
#include <vector>

class GameObject;
class ResourceInfo;

using std::vector;

class ComponentModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    ComponentModel(QObject *parent, GameObject* go);

    int             rowCount(const QModelIndex &parent = QModelIndex()) const;
    int             columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant        data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    //ResourceInfo*   getItem(const QModelIndex &index) const;

private:
    GameObject* m_gameObject;
};