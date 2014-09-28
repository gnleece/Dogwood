#include "DebugLogger.h"

#include <qtextedit.h>

void DebugLogger::SetTextEditTarget(QTextEdit* textEdit)
{
    m_textEdit = textEdit;
}

void DebugLogger::Log(string text)
{
    if (m_textEdit != NULL)
    {
        m_textEdit->append(QString(text.c_str()));
    }

    // TODO log to file
}