#pragma once

#include <QAbstractTableModel>
#include <vector>

class ComponentModelItem;
class ResourceInfo;
class ToolsideGameComponent;
class ToolsideGameObject;

using std::vector;

class ComponentModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    ComponentModel(QObject* parent, ToolsideGameObject* go);

    void            BuildModel();
    void            RefreshModel();
    void            ClearModel();

    ComponentModelItem* GetItem(const QModelIndex& index) const;

    QModelIndex     index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    QModelIndex     parent(const QModelIndex& index) const;

    int             rowCount(const QModelIndex& index = QModelIndex()) const;
    int             columnCount(const QModelIndex& index = QModelIndex()) const;

    QVariant        data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    bool            setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    Qt::ItemFlags   flags(const QModelIndex& index) const;

    Qt::DropActions supportedDragActions() const;
    Qt::DropActions supportedDropActions() const;

    QStringList     mimeTypes() const;
    bool            dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent);

private:
    bool            IsEditable(const QModelIndex& index) const;

    void            AddTransformData();
    void            AddMeshData();
    void            AddComponentData();

    ComponentModelItem*             m_rootItem;

    ToolsideGameObject*             m_gameObject;
    vector<ToolsideGameComponent*>  m_componentList;
};