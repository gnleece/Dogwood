#define GLEW_STATIC
#include <GL/glew.h>

#include "Widgets\AssetWidget.h"
#include "AssetDatabaseModel.h"
#include "DebugLogger.h"
#include "Scene\ResourceManager.h"

#include "..\GeneratedFiles\ui_assetwidget.h"

#include <qfileinfo.h>

AssetWidget::AssetWidget(QWidget* parent)
: m_ui(new Ui::AssetWidget)
{
    m_ui->setupUi(this);

    // Model setup
    m_model = new AssetDatabaseModel(this);
    m_ui->meshTableView->setModel(m_model);
    m_ui->meshTableView->show();

    // Button setup
    connect(m_ui->importButton, SIGNAL(clicked()), this, SLOT(ImportAssetClicked()));
}

void AssetWidget::ImportAssetClicked()
{
    // Display "open file" dialog
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Import Asset"),
                                                    "..\\Game\\Assets",
                                                    "Meshes (*.obj);;Textures (*.bmp);;Shaders (*.glsl)"
    );

    QFileInfo fileInfo(fileName);
    if (fileInfo.exists())
    {
        // Get info about selected file
        string suffix = fileInfo.suffix().toStdString();
        string absolutePath = fileName.toStdString();
        string projectPath = ResourceManager::Singleton().AbsolutePathToProjectPath(absolutePath);

        // Import the asset!
        bool success = ResourceManager::Singleton().ImportResource(projectPath, suffix);

        if (success)
        {
            DebugLogger::Singleton().Log("Successfully imported asset.");
        }
        else
        {
            DebugLogger::Singleton().Log("Error importing asset.");
        }
    }
}