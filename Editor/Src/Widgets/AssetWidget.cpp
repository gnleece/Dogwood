#define GLEW_STATIC
#include <GL/glew.h>

#include "Widgets\AssetWidget.h"
#include "AssetDatabaseModel.h"
#include "DebugLogger.h"
#include "MainEditorWindow.h"
#include "Scene\ResourceManager.h"

#include "..\GeneratedFiles\ui_assetwidget.h"

#include <qfileinfo.h>

AssetWidget::AssetWidget(MainEditorWindow* window, QWidget* parent)
: QWidget(parent), m_ui(new Ui::AssetWidget), m_window(window), m_sourceModel(NULL), m_proxyModel(NULL)
{
    m_ui->setupUi(this);
    m_ui->meshTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Add-to-object button setup
    connect(m_ui->addButton, SIGNAL(clicked()), this, SLOT(AddButtonClicked()));

    // Import button setup
    connect(m_ui->importButton, SIGNAL(clicked()), this, SLOT(ImportAssetClicked()));

    // Asset type filter button setup
    m_signalMapper = new QSignalMapper(this);
    connect(m_signalMapper, SIGNAL(mapped(int)), this, SLOT(ChangeTypeFilter(int)));

    m_filterButtons[ASSET_MESH] = m_ui->meshesButton;
    m_filterButtons[ASSET_TEXTURE] = m_ui->texturesButton;
    m_filterButtons[ASSET_SHADER] = m_ui->shadersButton;
    m_filterButtons[ASSET_SCRIPT] = m_ui->scriptsButton;
    m_filterNames[ASSET_MESH] = "Mesh";
    m_filterNames[ASSET_TEXTURE] = "Texture";
    m_filterNames[ASSET_SHADER] = "Shader";
    m_filterNames[ASSET_SCRIPT] = "Script";

    for (int i = 0; i < NUM_ASSET_TYPES; i++)
    {
        MapHookup(m_signalMapper, m_filterButtons[i], (eAssetType)i);
    }
}

void AssetWidget::Init()
{
    // Clear old model
    if (m_sourceModel != NULL)
    {
        delete m_sourceModel;
    }
    if (m_proxyModel != NULL)
    {
        delete m_proxyModel;
    }

    // Prepare new model
    m_sourceModel = new AssetDatabaseModel(this);
    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_sourceModel);
    m_proxyModel->setFilterRole(AssetDatabaseModel::FilterRole);
    m_ui->meshTableView->setModel(m_proxyModel);
    m_ui->meshTableView->show();

    m_proxyModel->sort(1);      // Sort based on path column
    ChangeTypeFilter(ASSET_MESH);
}

void AssetWidget::ImportAssetClicked()
{
    // Display "open file" dialog
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Import Asset"),
                                                    "..\\Game\\Assets",
                                                    "Meshes (*.obj);;Textures (*.bmp);;Shaders (*.glsl);;Scripts (*.h)"
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
            Init();     // TODO this rebuilds the whole model. fixme to just add new asset info
            m_window->SaveProject();
        }
        else
        {
            DebugLogger::Singleton().Log("Error importing asset.");
        }
    }
}

void AssetWidget::ChangeTypeFilter(int filter)
{
    m_proxyModel->setFilterRegExp(m_filterNames[filter].c_str());

    for (int i = 0; i < NUM_ASSET_TYPES; i++)
    {
        bool checked = (i == filter);
        m_filterButtons[i]->setChecked(checked);
    }
}

void AssetWidget::AddButtonClicked()
{
    if (!(m_ui->meshTableView->selectionModel()->selectedIndexes().isEmpty()))
    {
        QModelIndex proxyIndex = m_ui->meshTableView->selectionModel()->selectedIndexes().first();
        QModelIndex index = m_proxyModel->mapToSource(proxyIndex);
        ResourceInfo* info = m_sourceModel->getItem(index);
        if (info != NULL)
        {
            GameObject* go = m_window->GetSelectedObject();
            if (go != NULL)
            {
                info->AddToGameObject(go);
                // TODO refresh widgets
            }
        }
    }
}

void AssetWidget::MapHookup(QSignalMapper* signalMapper, QObject* sender, eAssetType assetType)
{
    connect(sender, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(sender, assetType);
}