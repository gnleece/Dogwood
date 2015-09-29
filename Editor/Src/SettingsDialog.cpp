#include "SettingsDialog.h"

#include "..\GeneratedFiles\ui_settingsdialog.h"

SettingsDialog::SettingsDialog(MainEditorWindow* window, QWidget* parent)
    : QDialog(parent), m_ui(new Ui::SettingsDialog), m_window(window)
{
    m_ui->setupUi(this);
}