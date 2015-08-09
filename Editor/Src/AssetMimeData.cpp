#include "AssetMimeData.h"

AssetMimeData::AssetMimeData(ResourceInfo* info)
{
    m_formats << "DogwoodEngine/AssetInfo";
    m_info = info;
}

QStringList AssetMimeData::formats() const
{
    return m_formats;
}

QVariant AssetMimeData::retrieveData(const QString &format, QVariant::Type preferredType) const
{
    if (format == "DogwoodEngine/GameObject")
    {
        return QVariant(QVariant::UserType, m_info);
    }
    else
    {
        return QMimeData::retrieveData(format, preferredType);
    }
}

ResourceInfo* AssetMimeData::GetResourceInfo()
{
    return m_info;
}