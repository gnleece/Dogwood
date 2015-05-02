#pragma once

#include <QAbstractTableModel>
#include <vector>

class GameObject;
class ResourceInfo;
class ToolsideGameComponent;

using std::vector;

class ComponentModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    ComponentModel(QObject *parent, GameObject* go);

    int             rowCount(const QModelIndex &parent = QModelIndex()) const;
    int             columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant        data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
    GameObject* m_gameObject;
    int         m_rowCount;
    vector<ToolsideGameComponent*> m_componentList;
    vector<int> m_accSizes;
};