#include "MainEditorWindow.h"
#include "DebugLogger.h"
#include "HierarchyModel.h"
#include "GameObject.h"
#include "GameProject.h"
#include "ui_maineditorwindow.h"
#include "Rendering\MeshInstance.h"
#include "Rendering\RenderManager.h"
#include "Scene\ResourceManager.h"
#include "Scene\Scene.h"
#include "Tools\TransformTool.h"
#include "Widgets\AssetWidget.h"
#include "Widgets\MeshWidget.h"
#include "Widgets\SceneViewWidget.h"
#include "Widgets\ScrollWidget.h"
#include "Widgets\TransformWidget.h"
#include "Widgets\WidgetUtils.h"

#include <QFileDialog>
#include <qinputdialog.h>
#include <QtWidgets>

using namespace EditorCommands;

MainEditorWindow::MainEditorWindow(QWidget *parent)
: m_ui(new Ui::MainEditorWindow), m_copiedGameObject(NULL), m_selectedGameObject(NULL),
  m_scene(NULL)
{
    // Window setup
    m_ui->setupUi(this);
    m_open = true;
    setWindowTitle(tr("[DOGWOOD EDITOR]"));

    // Tree view setup
    m_view = m_ui->treeView;

    // Asset widget setup
    m_assetWidget = new AssetWidget(this);
    m_ui->assetWidgetLayout->addWidget(m_assetWidget);

    // Scene view widget setup
    m_sceneViewWidget = new SceneViewWidget(this, this);
    m_sceneViewWidget->setFixedSize(990, 610);      // TODO set this properly
    m_ui->verticalLayout->addWidget(m_sceneViewWidget);

    // Game object widget (components list)
    SetupComponentWidgets();

    // Menu setup
    SetupMenuCommands();

    // Debug logging
    DebugLogger::Singleton().SetTextEditTarget(m_ui->textEdit_DebugOutput);

    showMaximized();
}

void MainEditorWindow::SetupComponentWidgets()
{
    // Parent widget, which holds all the component widgets
    ScrollWidget* componentsWidget = new ScrollWidget(m_ui->gameObjectScrollArea);
    m_ui->gameObjectScrollArea->setWidget(componentsWidget);

    // Transform widget
    m_transformWidget = new TransformWidget(componentsWidget, this);
    componentsWidget->AddChildWidget(m_transformWidget);
    m_transformWidget->hide();

    componentsWidget->AddChildWidget(AddLineSeparator(componentsWidget));

    // Mesh widget
    m_meshWidget = new MeshWidget(componentsWidget);
    componentsWidget->AddChildWidget(m_meshWidget);
    m_meshWidget->hide();
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

    // Edit menu
    connect(m_ui->actionUndo,                   SIGNAL(triggered()), this, SLOT(Undo()));
    connect(m_ui->actionRedo,                   SIGNAL(triggered()), this, SLOT(Redo()));

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

    // Mesh component menu
    m_addMeshSignalMapper = new QSignalMapper(this);
    connect(m_addMeshSignalMapper, SIGNAL(mapped(const QString &)), this, SLOT(AddMeshPrimitive(const QString &)));
    MapHookup(m_addMeshSignalMapper, m_ui->actionAdd_Cone_Mesh,     "mesh_cone");
    MapHookup(m_addMeshSignalMapper, m_ui->actionAdd_Cube_Mesh,     "mesh_cube");
    MapHookup(m_addMeshSignalMapper, m_ui->actionAdd_Cylinder_Mesh, "mesh_cylinder");
    MapHookup(m_addMeshSignalMapper, m_ui->actionAdd_Plane_Mesh,    "mesh_plane");
    MapHookup(m_addMeshSignalMapper, m_ui->actionAdd_Sphere_Mesh,   "mesh_sphere");
    MapHookup(m_addMeshSignalMapper, m_ui->actionAdd_Torus_Mesh,    "mesh_torus");
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
    connect(m_view->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(OnSelectionChanged(const QItemSelection &, const QItemSelection &)));
    m_view->expandAll();
}

GameObject* MainEditorWindow::GetSelectedObject()
{
    return m_selectedGameObject;
}

bool MainEditorWindow::IsOpen()
{
    return m_open;
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

void MainEditorWindow::NewProject()
{
    // Choose project name
    string projectName = QInputDialog::getText(this, "New Project", "Project Name").toStdString();

    // Choose project directory
    QString dirPath = QFileDialog::getExistingDirectory(this, "Choose project directory", "..");

    // Create Assets directory inside project directory
    QDir dir(dirPath);
    dir.mkdir("Assets");
    dir.mkpath("Assets/Meshes");
    dir.mkpath("Assets/Scenes");
    dir.mkpath("Assets/Shaders");
    dir.mkpath("Assets/Textures");

    // Set up new project and save it
    string projectFilename = dirPath.toStdString() + "/" + projectName + ".xml";
    string assetsPath = dirPath.toStdString() + "/Assets/";
    GameProject::Singleton().Unload();
    GameProject::Singleton().New(projectName, projectFilename, assetsPath);
    GameProject::Singleton().Save();

    // TODO unload scene, refresh scene view
}

void MainEditorWindow::OpenProject()
{
    // TODO implement me
}

void MainEditorWindow::SaveProject()
{
    if (GameProject::Singleton().IsLoaded());
    {
        GameProject::Singleton().Save();
        DebugLogger::Singleton().Log("Project saved.");
    }
}

void MainEditorWindow::NewScene()
{
    // TODO implement me
    DebugLogger::Singleton().Log("New scene: not implemented yet");
}

void MainEditorWindow::OpenScene()
{
    DebugLogger::Singleton().Log("Open scene");

    // Open file dialog
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Scene"), "", tr("Scene Files (*.xml)"));

    // TODO unload previous scene
    m_scene = new Scene();
    if (m_scene->Load(fileName.toStdString()))
    {
        HierarchyModel* model = new HierarchyModel(m_scene->GetRootObject());
        SetHierarchyModel(model);
        RenderManager::Singleton().SetRootObject(m_scene->GetRootObject());
        m_sceneViewWidget->SetScene(m_scene);
    }
    else
    {
        DebugLogger::Singleton().Log("Error loading scene");
        m_scene = NULL;
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
    
    m_scene = new Scene();
    if (m_scene->Load("..\\Game\\Assets\\Scenes\\Scene0.xml"))
    {
        HierarchyModel* model = new HierarchyModel(m_scene->GetRootObject());
        SetHierarchyModel(model);
        RenderManager::Singleton().SetRootObject(m_scene->GetRootObject());
        m_sceneViewWidget->SetScene(m_scene);
    }


    setWindowTitle(tr("[DOGWOOD EDITOR] Project: ") + QString(GameProject::Singleton().GetName().c_str()));
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
        m_scene->Save(fileName.toStdString());
    }
    else
    {
        DebugLogger::Singleton().Log("No scene to save.");
    }
}

void MainEditorWindow::CreateGameObject()
{
    DebugLogger::Singleton().Log("Creating game object");

    QModelIndex index = m_view->selectionModel()->selectedIndexes().first();
    CreateGameObjectCommand* command = new CreateGameObjectCommand(m_model, m_view, index);
    CommandManager::Singleton().ExecuteCommand(command);
}

void MainEditorWindow::DeleteGameObject()
{
    DebugLogger::Singleton().Log("Deleting game object");

    QModelIndex index = m_view->selectionModel()->selectedIndexes().first();
    DeleteGameObjectCommand* command = new DeleteGameObjectCommand(m_model, m_view, index);
    CommandManager::Singleton().ExecuteCommand(command);
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
        if (m_selectedGameObject->GetMesh() != NULL)
        {
            DebugLogger::Singleton().Log("Error: object already has a mesh component. Remove it before adding a new one.");
            return;
        }

        // TODO cleanup
        Mesh* mesh = (Mesh*)ResourceManager::Singleton().GetDefaultResource(meshName.toStdString());
        if (mesh != NULL)
        {
            MeshInstance* meshInstance = new MeshInstance();
            meshInstance->SetMesh(mesh);
            m_selectedGameObject->SetMesh(meshInstance);
            Material* material = new Material();
            meshInstance->SetMaterial(material);
            // TODO this breaks scene saving, because this shader has no guid
            material->SetShader(RenderManager::Singleton().GetCommonShader(RenderManager::eCommonShader::SHADER_GOURAUD));
        }
        else
        {
            DebugLogger::Singleton().Log("Error: mesh primitive missing from asset database.");
            return;
        }
    }
}

void MainEditorWindow::SelectObject(GameObject* gameObject)
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
}

void MainEditorWindow::OnSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{
    // Get the selected game object
    GameObject* selectedObject = NULL;
    if (!(m_view->selectionModel()->selectedIndexes().isEmpty()))
    {
        const QModelIndex index = selected.indexes().first();
        selectedObject = m_model->getItem(index);
    }
    SwitchSelectObject(selectedObject);
}

void MainEditorWindow::SwitchSelectObject(GameObject* gameobject)
{
    if (m_selectedGameObject != NULL)
    {
        m_selectedGameObject->SetSelected(false);
        m_selectedGameObject = NULL;
    }

    // TODO clean this up!
    if (gameobject != NULL)
    {
        // Show the components for the selected game object
        m_transformWidget->SetGameObject(gameobject);       // TODO add some kind of component widget base class to clean this up
        m_transformWidget->show();
        if (gameobject->GetMesh())
        {
            m_meshWidget->SetMeshInstance(gameobject->GetMesh());
            m_meshWidget->show();
        }
        else
        {
            m_meshWidget->SetMeshInstance(NULL);
            m_meshWidget->hide();
        }

        // Notify the game object that it's been selected
        gameobject->SetSelected(true);
        m_selectedGameObject = gameobject;
    }
    else
    {
        // Nothing is selected, so hide widgets
        m_transformWidget->SetGameObject(NULL);
        m_transformWidget->hide();
        m_meshWidget->SetMeshInstance(NULL);
        m_meshWidget->hide();
    }
}

void MainEditorWindow::resizeEvent(QResizeEvent* resizeEvent)
{
    QSize newSize = resizeEvent->size();
    int width = newSize.width();
    int height = newSize.height();

    DebugLogger::Singleton().Log("Window resize!");

    //TODO: scale/reposition widgets
}

void MainEditorWindow::closeEvent(QCloseEvent *event)
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

void MainEditorWindow::MapHookup(QSignalMapper* signalMapper, QObject* sender, QString text)
{
    connect(sender, SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(sender, text);
}