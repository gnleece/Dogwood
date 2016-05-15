#include "MainEditorWindow.h"
#include "DebugLogger.h"
#include "HierarchyModel.h"
#include "HierarchyView.h"
#include "GameProject.h"
#include "SettingsDialog.h"
#include "ToolsideGameComponent.h"
#include "ToolsideGameObject.h"
#include "ui_maineditorwindow.h"
#include "Rendering\MeshInstance.h"
#include "Rendering\RenderManager.h"
#include "Scene\ResourceManager.h"
#include "Scene\Scene.h"
#include "Tools\TransformTool.h"
#include "Widgets\AssetWidget.h"
#include "Widgets\ComponentWidget.h"
#include "Widgets\SceneViewWidget.h"
#include "Widgets\WidgetUtils.h"

#include <QFileDialog>
#include <qinputdialog.h>
#include <QtWidgets>

using namespace EditorCommands;
using std::string;

MainEditorWindow::MainEditorWindow(QWidget* parent)
: QMainWindow(parent), m_ui(new Ui::MainEditorWindow), m_copiedGameObject(NULL),
  m_selectedGameObject(NULL), m_scene(NULL)
{
    // Window setup
    m_ui->setupUi(this);
    m_open = true;
    setWindowTitle(tr("Dogwood Editor"));

    // Tree view setup
    m_view = new HierarchyView(this);
    m_ui->hierarchyViewLayout->addWidget(m_view);

    // Asset widget setup
    m_assetWidget = new AssetWidget(this);
    m_ui->assetWidgetLayout->addWidget(m_assetWidget);

    // Scene view widget setup
    m_sceneViewWidget = new SceneViewWidget(this, this);
    m_sceneViewWidget->setFixedSize(990, 610);      // TODO set this properly
    m_ui->verticalLayout->addWidget(m_sceneViewWidget);
    m_scene = Scene::New();

    // Game object widget (components list)
    m_componentWidget = new ComponentWidget(this);
    m_ui->componentViewLayout->addWidget(m_componentWidget);

    // Menu setup
    SetupMenuCommands();

    // Debug logging
    DebugLogger::Singleton().SetTextEditTarget(m_ui->textEdit_DebugOutput);

    // Dialog setup
    m_settingsDialog = new SettingsDialog(this);
    m_settingsDialog->setModal(true);
}

void MainEditorWindow::SetupMenuCommands()
{
    // File menu
    connect(m_ui->actionNew_Project,            SIGNAL(triggered()), this, SLOT(NewProject()));
    connect(m_ui->actionOpen_Project,           SIGNAL(triggered()), this, SLOT(OpenProject()));
    connect(m_ui->actionSave_Project,           SIGNAL(triggered()), this, SLOT(SaveProject()));
    connect(m_ui->actionNew_Scene,              SIGNAL(triggered()), this, SLOT(NewScene()));
    connect(m_ui->actionOpen_Scene,             SIGNAL(triggered()), this, SLOT(OpenScene()));
    connect(m_ui->actionSave_Scene,             SIGNAL(triggered()), this, SLOT(SaveScene()));
    connect(m_ui->actionSave_Scene_As,          SIGNAL(triggered()), this, SLOT(SaveSceneAs()));
    connect(m_ui->actionSave_All,               SIGNAL(triggered()), this, SLOT(SaveAll()));

    // Edit menu
    connect(m_ui->actionUndo,                   SIGNAL(triggered()), this, SLOT(Undo()));
    connect(m_ui->actionRedo,                   SIGNAL(triggered()), this, SLOT(Redo()));

    // Settings menu
    connect(m_ui->actionProject_Settings,       SIGNAL(triggered()), this, SLOT(ShowProjectSettingsDialog()));

    // Game Object menu
    connect(m_ui->actionCreate_Game_Object,     SIGNAL(triggered()), this, SLOT(CreateGameObject()));
    connect(m_ui->actionDelete_Game_Object,     SIGNAL(triggered()), this, SLOT(DeleteGameObject()));
    connect(m_ui->actionCopy_Game_Object,       SIGNAL(triggered()), this, SLOT(CopyGameObject()));
    connect(m_ui->actionCut_Game_Object,        SIGNAL(triggered()), this, SLOT(CutGameObject()));
    connect(m_ui->actionPaste_Game_Object,      SIGNAL(triggered()), this, SLOT(PasteGameObject()));

    // Debug menu
    connect(m_ui->actionOpen_Test_Project,      SIGNAL(triggered()), this, SLOT(OpenTestProject()));

    // Transform tool buttons
    connect(m_ui->transformButton_Translate,    SIGNAL(clicked()),   this, SLOT(TransformTranslateButton()));
    connect(m_ui->transformButton_Rotate,       SIGNAL(clicked()),   this, SLOT(TransformRotateButton()));
    connect(m_ui->transformButton_Scale,        SIGNAL(clicked()),   this, SLOT(TransformScaleButton()));

    // Component menu
    connect(m_ui->actionRebuild_Script_Info,    SIGNAL(triggered()), this, SLOT(RebuildComponentSchema()));
    connect(m_ui->actionRebuild_Shader_Info,    SIGNAL(triggered()), this, SLOT(RebuildShaderSchema()));

    // Mesh component menu
    m_addMeshSignalMapper = new QSignalMapper(this);
    connect(m_addMeshSignalMapper, SIGNAL(mapped(const QString &)), this, SLOT(AddMeshPrimitive(const QString &)));
    SignalMapHookup(m_addMeshSignalMapper, m_ui->actionAdd_Cone_Mesh,     "mesh_cone");
    SignalMapHookup(m_addMeshSignalMapper, m_ui->actionAdd_Cube_Mesh,     "mesh_cube");
    SignalMapHookup(m_addMeshSignalMapper, m_ui->actionAdd_Cylinder_Mesh, "mesh_cylinder");
    SignalMapHookup(m_addMeshSignalMapper, m_ui->actionAdd_Plane_Mesh,    "mesh_plane");
    SignalMapHookup(m_addMeshSignalMapper, m_ui->actionAdd_Sphere_Mesh,   "mesh_sphere");
    SignalMapHookup(m_addMeshSignalMapper, m_ui->actionAdd_Torus_Mesh,    "mesh_torus");

    // Physics component menu
    m_addPhysicsSignalMapper = new QSignalMapper(this);
    connect(m_addPhysicsSignalMapper, SIGNAL(mapped(int)), this, SLOT(AddCollider(int)));
    SignalMapHookup(m_addPhysicsSignalMapper, m_ui->actionAdd_Sphere_Collider,  (int)Collider::SPHERE_COLLIDER);
    SignalMapHookup(m_addPhysicsSignalMapper, m_ui->actionAdd_Box_Collider,     (int)Collider::BOX_COLLIDER);
    SignalMapHookup(m_addPhysicsSignalMapper, m_ui->actionAdd_Capsule_Collider, (int)Collider::CAPSULE_COLLIDER);

    // Effect menu
    connect(m_ui->actionParticle_System, SIGNAL(triggered()), this, SLOT(AddParticleSystem()));
}

MainEditorWindow::~MainEditorWindow()
{
    delete m_ui;
    delete m_sceneViewWidget;
}

void MainEditorWindow::PostSetup()
{
    m_sceneViewWidget->PostSetup();
}

void MainEditorWindow::Update()
{
    // Enable/disable menu options based on current state
    UpdateMenuState();

    // Update scene view
    m_sceneViewWidget->update();
}

void MainEditorWindow::SetHierarchyModel(HierarchyModel* model)
{
    m_model = model;
    m_view->setModel(m_model);
    DebugLogger::Singleton().Log("Hierarchy model set!");
    connect(m_view, SIGNAL(SelectedObjectChanged(QModelIndex&)), this, SLOT(OnHierarchySelectionChanged(QModelIndex&)));
    m_view->expandAll();
}

ToolsideGameObject* MainEditorWindow::GetSelectedObject()
{
    return m_selectedGameObject;
}

bool MainEditorWindow::IsOpen()
{
    return m_open;
}

void MainEditorWindow::Refresh()
{
    m_componentWidget->Refresh();
}

// TODO: Qt textboxes seem to capture ctrl + z and do their own undo. stop that!
// TODO: just selecting a game object counts as a bunch of update commands. fix that!
void MainEditorWindow::Undo()
{
    // TODO disable menu option if undo is not currently available
    bool success = CommandManager::Singleton().Undo();

    if (success)
    {
        DebugLogger::Singleton().Log("Undo");
    }
    else
    {
        DebugLogger::Singleton().Log("Can't undo. Stack is empty.");
    }
}

void MainEditorWindow::Redo()
{
    // TODO disable menu option if redo is not currently available
    bool success = CommandManager::Singleton().Redo();

    if (success)
    {
        DebugLogger::Singleton().Log("Redo");
    }
    else
    {
        DebugLogger::Singleton().Log("Can't redo. Stack is empty.");
    }
}

void MainEditorWindow::ShowProjectSettingsDialog()
{
    DebugLogger::Singleton().Log("Opening project settings dialog...");

    m_settingsDialog->show();
}

void MainEditorWindow::NewProject()
{
    // Choose project name
    QString projectName = QInputDialog::getText(this, "New Project", "Project Name");

    if (projectName.isNull())
        return;

    // Choose project directory
    QString dirPath = QFileDialog::getExistingDirectory(this, "Choose project directory", "..");

    if (dirPath.isNull())
        return;

    // Create Assets directory inside project directory
    QDir dir(dirPath);
    dir.mkdir("Assets");
    dir.mkpath("Assets/Meshes");
    dir.mkpath("Assets/Scenes");
    dir.mkpath("Assets/Shaders");
    dir.mkpath("Assets/Textures");

    // Set up new project and save it
    string projectNameString = projectName.toStdString();
    string projectFilename = dirPath.toStdString() + "/" + projectNameString + ".xml";
    string assetsPath = dirPath.toStdString() + "/Assets/";
    GameProject::Singleton().Unload();
    GameProject::Singleton().New(projectNameString, projectFilename, assetsPath);
    GameProject::Singleton().Save();

    m_assetWidget->Init();

    NewScene();
}

void MainEditorWindow::OpenProject()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"), "..", tr("Dogwood Projects (*.xml)"));

    if (fileName.isNull())
        return;

    GameProject::Singleton().Unload();

    bool success = GameProject::Singleton().Load(fileName.toStdString());
    if (!success)
    {
        DebugLogger::Singleton().Log("Error loading project: " + fileName.toStdString());
        return;
    }

    // TODO load startup scene (or last opened scene)
    m_sceneViewWidget->SetScene(NULL);

    setWindowTitle(tr("Dogwood Editor - ") + QString(GameProject::Singleton().GetName().c_str()));
    m_assetWidget->Init();
}

void MainEditorWindow::SaveProject()
{
    if (GameProject::Singleton().IsLoaded())
    {
        GameProject::Singleton().Save();
        DebugLogger::Singleton().Log("Project saved.");
    }
}

void MainEditorWindow::RebuildComponentSchema()
{
    DebugLogger::Singleton().Log("Rebuilding component schema. Running ProcessScripts.py...");

    // Save project file, to make sure any newly imported scripts are added to the list
    GameProject::Singleton().Save();

    QProcess *process = new QProcess(this);
    string cmd = string("python Scripts/ProcessScripts.py ") + GameProject::Singleton().GetFilename();
    process->start(QString(cmd.c_str()));
    process->waitForFinished();

    ResourceManager::Singleton().LoadComponentSchema();

    DebugLogger::Singleton().Log("Done rebuilding schema.");
}

void MainEditorWindow::RebuildShaderSchema()
{
    DebugLogger::Singleton().Log("Rebuilding shader schema. Running ProcessShaders.py...");

    // Save project file, to make sure any newly imported shaders are added to the list
    GameProject::Singleton().Save();

    QProcess *process = new QProcess(this);
    string cmd = string("python Scripts/ProcessShaders.py ") + GameProject::Singleton().GetFilename();
    process->start(QString(cmd.c_str()));
    process->waitForFinished();

    ResourceManager::Singleton().LoadShaderSchema();

    DebugLogger::Singleton().Log("Done rebuilding schema.");
}

void MainEditorWindow::NewScene()
{
    UnloadScene();

    // Open file dialog
    string defaultPath = GameProject::Singleton().GetResourceBasePath();
    QString fileName = QFileDialog::getSaveFileName(this, "New Scene", defaultPath.c_str(), "*.xml");

    if (fileName.isNull())
        return;

    m_scene->New(fileName.toStdString() + ".xml");

    // Open the new scene
    HierarchyModel* model = new HierarchyModel(m_scene->GetToolsideRootObject());
    SetHierarchyModel(model);
    RenderManager::Singleton().SetRootObject(m_scene->GetRootObject());
    m_sceneViewWidget->SetScene(m_scene);
}

void MainEditorWindow::OpenScene()
{
    DebugLogger::Singleton().Log("Open scene");

    // Open file dialog
    string defaultPath = GameProject::Singleton().GetResourceBasePath();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Scene"), defaultPath.c_str(), tr("Scene Files (*.xml)"));

    if (fileName.isNull())
        return;

    UnloadScene();
    m_scene = Scene::Load(fileName.toStdString());

    if (m_scene != NULL)
    {
        HierarchyModel* model = new HierarchyModel(m_scene->GetToolsideRootObject());
        SetHierarchyModel(model);
        RenderManager::Singleton().SetRootObject(m_scene->GetRootObject());
        m_sceneViewWidget->SetScene(m_scene);
    }
    else
    {
        DebugLogger::Singleton().Log("Error loading scene");
    }
}

void MainEditorWindow::UnloadScene()
{
    if (m_scene != NULL && m_scene->IsLoaded())
    {
        // TODO prompt user to save if there are unsaved changes
        Scene::Unload(m_scene);
    }
}

void MainEditorWindow::OpenTestProject()
{
    GameProject::Singleton().Unload();

    bool success = GameProject::Singleton().Load("..\\Game\\Katamari.xml");
    if (!success)
    {
        DebugLogger::Singleton().Log("Error loading test project.");
        return;
    }
    
    UnloadScene();
    m_scene = Scene::Load("..\\Game\\Assets\\Scenes\\RollTest.xml");

    if (m_scene != NULL)
    {
        HierarchyModel* model = new HierarchyModel(m_scene->GetToolsideRootObject());
        SetHierarchyModel(model);
        RenderManager::Singleton().SetRootObject(m_scene->GetRootObject());
        m_sceneViewWidget->SetScene(m_scene);
    }

    setWindowTitle(tr("Dogwood Editor - ") + QString(GameProject::Singleton().GetName().c_str()));
    m_assetWidget->Init();
}

void MainEditorWindow::SaveScene()
{
    DebugLogger::Singleton().Log("Save scene");

    if (m_scene != NULL)
    {
        m_scene->Save();
    }
}

void MainEditorWindow::SaveSceneAs()
{
    DebugLogger::Singleton().Log("Save scene as...");

    if (m_scene != NULL)
    {
        // Open file dialog
        QString fileName = QFileDialog::getSaveFileName(this, "Save Scene As", "", "*.xml");
        if (!fileName.isNull())
        {
            m_scene->Save(fileName.toStdString());
        }
    }
    else
    {
        DebugLogger::Singleton().Log("No scene to save.");
    }
}

void MainEditorWindow::SaveAll()
{
    SaveProject();
    SaveScene();
}

void MainEditorWindow::CreateGameObject()
{
    DebugLogger::Singleton().Log("Creating game object");

    QModelIndex index;
    if (!(m_view->selectionModel()->selectedIndexes().isEmpty()))
    {
        index = m_view->selectionModel()->selectedIndexes().first();
    }
    else
    {
        index = QModelIndex();      // create object at root level
    }

    CreateGameObjectCommand* command = new CreateGameObjectCommand(m_model, m_view, index);
    CommandManager::Singleton().ExecuteCommand(command);
}

void MainEditorWindow::DeleteGameObject()
{
    DebugLogger::Singleton().Log("Deleting game object");

    if (!(m_view->selectionModel()->selectedIndexes().isEmpty()))
    {
        QModelIndex index = m_view->selectionModel()->selectedIndexes().first();
        DeleteGameObjectCommand* command = new DeleteGameObjectCommand(m_model, m_view, index);
        CommandManager::Singleton().ExecuteCommand(command);
    }
}

void MainEditorWindow::CopyGameObject()
{
    if (!(m_view->selectionModel()->selectedIndexes().isEmpty()))
    {
        QModelIndex index = m_view->selectionModel()->selectedIndexes().first();

        m_copiedGameObject = index.isValid() ? m_model->getItem(index) : NULL;

        if (m_copiedGameObject != NULL)
        {
            DebugLogger::Singleton().Log("Copied game object");
        }
        else
        {
            DebugLogger::Singleton().Log("Tried to copy game object but selection was invalid.");
        }
    }
}

void MainEditorWindow::CutGameObject()
{
    if (!(m_view->selectionModel()->selectedIndexes().isEmpty()))
    {
        QModelIndex index = m_view->selectionModel()->selectedIndexes().first();

        m_copiedGameObject = index.isValid() ? m_model->getItem(index) : NULL;

        if (m_copiedGameObject != NULL)
        {
            DebugLogger::Singleton().Log("Cutting game object");

            DeleteGameObjectCommand* command = new DeleteGameObjectCommand(m_model, m_view, index);
            CommandManager::Singleton().ExecuteCommand(command);
        }
        else
        {
            DebugLogger::Singleton().Log("Tried to cut object but selection was invalid.");
        }
    }
}

void MainEditorWindow::PasteGameObject()
{
    if (m_copiedGameObject != NULL && !(m_view->selectionModel()->selectedIndexes().isEmpty()))
    {
        DebugLogger::Singleton().Log("Pasting game object");

        QModelIndex index = m_view->selectionModel()->selectedIndexes().first();
        PasteGameObjectCommand* command = new PasteGameObjectCommand(m_model, m_view, index, m_copiedGameObject);
        CommandManager::Singleton().ExecuteCommand(command);
    }
    else
    {
        DebugLogger::Singleton().Log("Tried to paste but there was no copied object.");
    }
}

void MainEditorWindow::TransformTranslateButton()
{
    m_sceneViewWidget->SetTransformToolMode(TransformTool::eMode::TOOL_MODE_TRANSLATE);
    m_ui->transformButton_Translate->setChecked(true);
    m_ui->transformButton_Rotate->setChecked(false);
    m_ui->transformButton_Scale->setChecked(false);
}

void MainEditorWindow::TransformRotateButton()
{
    m_sceneViewWidget->SetTransformToolMode(TransformTool::eMode::TOOL_MODE_ROTATE);
    m_ui->transformButton_Translate->setChecked(false);
    m_ui->transformButton_Rotate->setChecked(true);
    m_ui->transformButton_Scale->setChecked(false);
}

void MainEditorWindow::TransformScaleButton()
{
    m_sceneViewWidget->SetTransformToolMode(TransformTool::eMode::TOOL_MODE_SCALE);
    m_ui->transformButton_Translate->setChecked(false);
    m_ui->transformButton_Rotate->setChecked(false);
    m_ui->transformButton_Scale->setChecked(true);
}

void MainEditorWindow::AddMeshPrimitive(const QString& meshName)
{
    if (m_selectedGameObject != NULL)
    {
        if (m_selectedGameObject->GetMeshInstance() != NULL)
        {
            DebugLogger::Singleton().Log("Error: object already has a mesh component. Remove it before adding a new one.");
            return;
        }

        Resource* mesh = ResourceManager::Singleton().GetDefaultResource(meshName.toStdString());
        mesh->GetResourceInfo()->AddToGameObject(m_selectedGameObject);
    }
}

void MainEditorWindow::AddCollider(int type)
{
    if (m_selectedGameObject != NULL)
    {
        Collider::AddToGameObject(m_selectedGameObject, (Collider::ColliderType)type);
    }
}

void MainEditorWindow::AddParticleSystem()
{
    if (m_selectedGameObject != NULL)
    {
        // TODO hardcoding this guid is so, so ugly
        ToolsideGameComponent* component = new ToolsideGameComponent();
        component->Create(248907459, true);
        m_selectedGameObject->AddComponent(component);
    }
}

// Used to select an object from outside the hierarchy view itself (e.g. from the scene view)
void MainEditorWindow::SelectObject(ToolsideGameObject* gameObject)
{
    // Clear previous selection
    m_view->selectionModel()->clearSelection();
    if (m_selectedGameObject)
    {
        m_selectedGameObject->SetSelected(false);
    }
    m_selectedGameObject = NULL;

    // Search the model for the given game object
    if (gameObject)
    {
        QModelIndexList Items = m_model->match(
            m_model->index(0, 0),                       // start
            HierarchyModel::MatchRole,                  // role
            QVariant::fromValue(gameObject->GetID()),   // value
            1,                                          // hits
            Qt::MatchRecursive);                        // flags

        // Select new object in hierarchy view
        m_view->selectionModel()->select(Items.first(), QItemSelectionModel::Select);
    }
    m_view->UpdateSelectedObject();
}

void MainEditorWindow::OnHierarchySelectionChanged(QModelIndex& newIndex)
{
    // Get the selected game object
    ToolsideGameObject* selectedObject = m_model->getItem(newIndex);
    SwitchSelectObject(selectedObject);
}

void MainEditorWindow::SwitchSelectObject(ToolsideGameObject* gameobject)
{
    if (m_selectedGameObject != NULL)
    {
        m_selectedGameObject->SetSelected(false);
        m_selectedGameObject = NULL;
    }

    // TODO clean this up!
    if (gameobject != NULL)
    {
        // Initialize the component widget
        m_componentWidget->Init(gameobject);
        m_componentWidget->show();

        // Notify the game object that it's been selected
        gameobject->SetSelected(true);
        m_selectedGameObject = gameobject;
    }
    else
    {
        m_componentWidget->hide();  // TODO reset
    }
}

void MainEditorWindow::resizeEvent(QResizeEvent* /*resizeEvent*/)
{
    //QSize newSize = resizeEvent->size();
    //int width = newSize.width();
    //int height = newSize.height();

    //DebugLogger::Singleton().Log("Window resize!");

    //TODO: scale/reposition widgets
}

void MainEditorWindow::closeEvent(QCloseEvent* /*event*/)
{
    m_open = false;
}

void MainEditorWindow::UpdateMenuState()
{
    bool projectLoaded = GameProject::Singleton().IsLoaded();

    // Only enable scene operations if a project is loaded
    m_ui->actionNew_Scene->setEnabled(projectLoaded);
    m_ui->actionOpen_Scene->setEnabled(projectLoaded);
    m_ui->actionSave_Scene->setEnabled(projectLoaded && m_scene);
    m_ui->actionSave_Scene_As->setEnabled(projectLoaded && m_scene);
    m_ui->actionSave_Project->setEnabled(projectLoaded);

    // Only enable undo/redo if there are commands on the stack
    m_ui->actionUndo->setEnabled(CommandManager::Singleton().CanUndo());
    m_ui->actionRedo->setEnabled(CommandManager::Singleton().CanRedo());

    // Only enable object creation & paste if a scene is open
    m_ui->actionCreate_Game_Object->setEnabled(m_scene);
    m_ui->actionPaste_Game_Object->setEnabled(m_scene && m_copiedGameObject);

    // Only enable object delete/cut/copy if a scene is open and an object is selected
    m_ui->actionDelete_Game_Object->setEnabled(m_scene && m_selectedGameObject);
    m_ui->actionCopy_Game_Object->setEnabled(m_scene && m_selectedGameObject);
    m_ui->actionCut_Game_Object->setEnabled(m_scene && m_selectedGameObject);
}

void MainEditorWindow::SignalMapHookup(QSignalMapper* signalMapper, QObject* sender, QString text)
{
    connect(sender, SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(sender, text);
}

void MainEditorWindow::SignalMapHookup(QSignalMapper* signalMapper, QObject* sender, int value)
{
    connect(sender, SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(sender, value);
}