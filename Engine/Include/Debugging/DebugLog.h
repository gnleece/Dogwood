#pragma once

#include <string>
#include <iostream>
#include <fstream>

using std::ofstream;
using std::string;

class DebugLog
{
public:
    enum LogType
    {
        Standard = 1,
        Rendering = 2,
        Physics = 4,
        GameObject = 8,
        Assets = 16
    };
    const int ALL_TYPES = Standard | Rendering | Physics | GameObject;

    static DebugLog& Singleton()
    {
        static DebugLog singleton;
        return singleton;
    }
    DebugLog() {}

    void        Startup(string logFilepath = "");
    void        Shutdown();

    void        Log(string text, LogType type);
    void        SetEnabledTypes(int typeBitMask);
    void        SetLogFile(string logFilepath);
    void        EnableStdOut(bool enable);

private:
    int         m_enabledTypes;
    bool        m_stdOutEnabled;
    ofstream    m_logfile;
};