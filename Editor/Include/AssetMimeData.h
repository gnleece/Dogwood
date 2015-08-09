#pragma once

#include <QMimeData>

class ResourceInfo;

class AssetMimeData : public QMimeData
{
    Q_OBJECT

public:
    AssetMimeData(ResourceInfo* info);

    QStringList     formats() const;
    QVariant        retrieveData(const QString &format, QVariant::Type preferredType) const;

    ResourceInfo*   GetResourceInfo();

private:
    ResourceInfo*   m_info;
    QStringList     m_formats;
};