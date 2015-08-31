#pragma once

#include <QAbstractItemModel>
#include <string>

class ToolsideGameObject;

using std::string;

class HierarchyModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    HierarchyModel(ToolsideGameObject* root, QObject *parent = 0);
    ~HierarchyModel();

    QModelIndex         index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex         parent(const QModelIndex &index) const;

    QVariant            data(const QModelIndex& index, int role) const;
    bool                setData(const QModelIndex& index, const QVariant &value, int role = Qt::EditRole);
    void                setItemName(const QModelIndex& index, string name);
    Qt::ItemFlags       flags(const QModelIndex& index) const;

    int                 columnCount(const QModelIndex& index = QModelIndex()) const;

    int                 rowCount(const QModelIndex& index = QModelIndex()) const;
    bool                insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
    bool                insertChild(QModelIndex parentIndex, ToolsideGameObject* parent, ToolsideGameObject* child, int position);
    bool                removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

    Qt::DropActions     supportedDragActions() const;
    Qt::DropActions     supportedDropActions() const;
    QStringList         mimeTypes() const;
    QMimeData*          mimeData(const QModelIndexList &indexes) const;
    bool                dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

    ToolsideGameObject* getItem(const QModelIndex &index) const;

    const static int    MatchRole = 32;

private:
    ToolsideGameObject* m_rootItem;
};