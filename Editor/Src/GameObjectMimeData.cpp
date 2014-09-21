#include "GameObjectMimeData.h"

GameObjectMimeData::GameObjectMimeData(GameObject* gameObject)
{
    m_formats << "DogwoodEngine/GameObject";
    m_gameObject = gameObject;
}

QStringList GameObjectMimeData::formats() const
{
    return m_formats;
}

QVariant GameObjectMimeData::retrieveData(const QString &format, QVariant::Type preferredType) const
{
    if (format == "DogwoodEngine/GameObject")
    {
        return QVariant(QVariant::UserType, m_gameObject);
    }
    else
    {
        return QMimeData::retrieveData(format, preferredType);
    }
}

GameObject* GameObjectMimeData::getGameObject()
{
    return m_gameObject;
}