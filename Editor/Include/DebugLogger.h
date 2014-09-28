#pragma once

#include <string>

using std::string;

class QTextEdit;

class DebugLogger
{
public:
    static DebugLogger& Singleton()
    {
        static DebugLogger singleton;
        return singleton;
    }
    DebugLogger() {}

    void SetTextEditTarget(QTextEdit* textEdit);
    //void SetFileTarget();     // TODO implement me

    void Log(string text);

private:
    QTextEdit*  m_textEdit;
};