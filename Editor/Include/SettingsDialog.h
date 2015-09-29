#pragma once

#include <QDialog>
#include <string>

class MainEditorWindow;

using std::string;

namespace Ui
{
    class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    SettingsDialog(MainEditorWindow* window, QWidget* parent = 0);

private:
    Ui::SettingsDialog*     m_ui;
    MainEditorWindow*       m_window;

private slots:
};