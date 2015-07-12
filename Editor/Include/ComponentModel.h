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
    bool            setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags   flags(const QModelIndex &index) const;

    Qt::DropActions supportedDragActions() const;
    Qt::DropActions supportedDropActions() const;

    QStringList     mimeTypes() const;
    bool            dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

private:
    int             CalculateComponentIndex(int row) const;
    int             CalculateParamIndex(int row, int componentIndex) const;
    bool            IsEditable(const QModelIndex &index) const;

    GameObject* m_gameObject;
    int         m_rowCount;
    vector<ToolsideGameComponent*> m_componentList;
    vector<int> m_accSizes;
};