#include "SettingsDialog.h"
#include "GeneratedFiles\ui_settingsdialog.h"

#include <QPlainTextEdit>
#include "GameProject.h"

SettingsDialog::SettingsDialog(MainEditorWindow* window, QWidget* parent)
    : QDialog(parent), m_ui(new Ui::SettingsDialog), m_window(window)
{
    // TODO populate UI with current settings values
    m_ui->setupUi(this);
}

void SettingsDialog::accept()
{
    int width = m_ui->resolutionWidth->toPlainText().toInt();
    int height = m_ui->resolutionHeight->toPlainText().toInt();
    GameProject::Singleton().SetResolution(width, height);

    bool physicsEnabled = m_ui->physicsEnabled;
    float gravity = m_ui->physicsGravity->toPlainText().toFloat();
    GameProject::Singleton().SetPhysicsSettings(GameProject::PhysicsSettings(physicsEnabled, gravity));

    QDialog::accept();
}