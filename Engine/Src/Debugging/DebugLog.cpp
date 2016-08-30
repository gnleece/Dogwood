#include "Debugging/DebugLog.h"

void DebugLog::Startup()
{
    m_enabledTypes = ALL_TYPES;
}

void DebugLog::Shutdown()
{

}

void DebugLog::Log(string text, LogType type)
{
    // TODO implement me
}

void DebugLog::SetEnabledTypes(int typeBitMask)
{
    m_enabledTypes = typeBitMask;
}