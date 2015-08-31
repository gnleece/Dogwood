#pragma once

#include <QMimeData>
#include <QModelIndex>

class QTreeView;
class ToolsideGameObject;

class GameObjectMimeData : public QMimeData
{
    Q_OBJECT

public:
    GameObjectMimeData(ToolsideGameObject* gameObject, int originalRow, QModelIndex& originalParentIndex);

    QStringList         formats() const;
    QVariant            retrieveData(const QString &format, QVariant::Type preferredType) const;

    ToolsideGameObject* getGameObject();
    int                 getOriginalRow();
    QModelIndex&        getOriginalParentIndex();

private:
    ToolsideGameObject* m_gameObject;           // TODO support list of game objects
    int                 m_originalRow;
    QModelIndex         m_originalParentIndex;

    QStringList         m_formats;
};