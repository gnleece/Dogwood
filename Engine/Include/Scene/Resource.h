#pragma once

struct ResourceInfo;

class Resource
{
public:
    ResourceInfo* GetResourceInfo()     { return m_resourceInfo; }

protected:
    ResourceInfo* m_resourceInfo;
};
