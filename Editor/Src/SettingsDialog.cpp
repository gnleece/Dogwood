#include "SettingsDialog.h"
#include "..\GeneratedFiles\ui_settingsdialog.h"

#include <QPlainTextEdit>
#include "GameProject.h"

SettingsDialog::SettingsDialog(MainEditorWindow* window, QWidget* parent)
    : QDialog(parent), m_ui(new Ui::SettingsDialog), m_window(window)
{
    m_ui->setupUi(this);
}

void SettingsDialog::accept()
{
    int width = m_ui->resolutionWidth->toPlainText().toInt();
    int height = m_ui->resolutionHeight->toPlainText().toInt();

    GameProject::Singleton().SetResolution(width, height);

    QDialog::accept();
}