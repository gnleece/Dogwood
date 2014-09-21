#pragma once

#include <QMimeData>
#include <QModelIndex>

class GameObject;

class GameObjectMimeData : public QMimeData
{
    Q_OBJECT

public:
    GameObjectMimeData(GameObject* gameObject, int originalRow, QModelIndex& originalParentIndex);

    QStringList     formats() const;
    QVariant        retrieveData(const QString &format, QVariant::Type preferredType) const;

    GameObject*     getGameObject();
    int             getOriginalRow();
    QModelIndex&    getOriginalParentIndex();

private:
    GameObject*     m_gameObject;           // TODO support list of gameobjects
    int             m_originalRow;
    QModelIndex     m_originalParentIndex;

    QStringList     m_formats;
};