#pragma once

#include <QMimeData>

class GameObject;

class GameObjectMimeData : public QMimeData
{
    Q_OBJECT

public:
    GameObjectMimeData(GameObject* gameObject);

    QStringList formats() const;
    QVariant    retrieveData(const QString &format, QVariant::Type preferredType) const;
    GameObject* getGameObject();

private:
    GameObject* m_gameObject;   // TODO support list of gameobjects
    QStringList m_formats;
};