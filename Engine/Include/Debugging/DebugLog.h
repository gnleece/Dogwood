#pragma once

#include <string>

using std::string;

class DebugLog
{
public:
    enum LogType
    {
        Rendering = 1,
        Physics = 2,
        GameObject = 4,
        Other = 8
    };
    const int ALL_TYPES = Rendering | Physics | GameObject | Other;

    static DebugLog& Singleton()
    {
        static DebugLog singleton;
        return singleton;
    }
    DebugLog() {}

    void    Startup();
    void    Shutdown();

    void    Log(string text, LogType type);
    void    SetEnabledTypes(int typeBitMask);

private:
    int     m_enabledTypes;
};