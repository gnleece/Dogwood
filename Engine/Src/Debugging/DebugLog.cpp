#include "Debugging/DebugLog.h"

using std::cout;

void DebugLog::Startup(string logFilepath)
{
    m_enabledTypes = ALL_TYPES;
    m_stdOutEnabled = true;
    SetLogFile(logFilepath);
}

void DebugLog::Shutdown()
{
    if (m_logfile.is_open())
    {
        m_logfile.close();
    }
}

void DebugLog::Log(string text, LogType type)
{
    if (type & m_enabledTypes)
    {
        if (m_stdOutEnabled)
        {
            cout << text;
        }

        if (m_logfile.is_open())
        {
            m_logfile << text;
        }
    }
}

void DebugLog::SetEnabledTypes(int typeBitMask)
{
    m_enabledTypes = typeBitMask;
}

void DebugLog::SetLogFile(string logFilepath)
{
    if (m_logfile.is_open())
    {
        m_logfile.close();
    }

    if (logFilepath != "")
    {
        m_logfile.open(logFilepath);
    }
}