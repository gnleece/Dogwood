#include "GameObjectMimeData.h"

GameObjectMimeData::GameObjectMimeData(ToolsideGameObject* gameObject, int originalRow, QModelIndex& originalParentIndex)
{
    m_formats << "DogwoodEngine/GameObject";
    m_gameObject = gameObject;
    m_originalRow = originalRow;
    m_originalParentIndex = originalParentIndex;
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

ToolsideGameObject* GameObjectMimeData::getGameObject()
{
    return m_gameObject;
}

int GameObjectMimeData::getOriginalRow()
{
    return m_originalRow;
}

QModelIndex& GameObjectMimeData::getOriginalParentIndex()
{
    return m_originalParentIndex;
}