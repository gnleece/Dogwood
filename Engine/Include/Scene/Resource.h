#pragma once

class ResourceInfo;

class Resource
{
public:
    ResourceInfo* GetResourceInfo()     { return m_resourceInfo; }

protected:
    ResourceInfo* m_resourceInfo;
};
