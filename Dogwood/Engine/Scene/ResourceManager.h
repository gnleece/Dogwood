#pragma once

class ResourceManager
{
public:
    static ResourceManager& Singleton()
    {
        static ResourceManager    singleton;
        return singleton;
    }
    ResourceManager() {}

    void Startup();
    void Shutdown();
};