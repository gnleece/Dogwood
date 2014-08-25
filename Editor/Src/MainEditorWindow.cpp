#include "maineditorwindow.h"
#include "HierarchyModel.h"
#include "GameObject.h"
#include "ui_maineditorwindow.h"
#include "Widgets\GLWidget.h"
#include "Widgets\TransformWidget.h"

#include <QtWidgets>

using namespace EditorCommands;

MainEditorWindow::MainEditorWindow(QWidget *parent)
: m_ui(new Ui::MainEditorWindow)
{
    // Window setup
    m_ui->setupUi(this);
    setWindowTitle(tr("Dogwood Editor!"));
    m_ui->textEdit_DebugOutput->append("Loading scene...");

    // Tree view setup
    m_view = m_ui->treeView;

    // OpenGL widget setup
    m_glWidget = new GLWidget;
    m_glWidget->setFixedSize(640, 480);
    m_ui->verticalLayout->addWidget(m_glWidget);

    // Component widgets
    m_transformWidget = new TransformWidget(this, this);
    m_ui->componentLayout->addWidget(m_transformWidget);
    m_transformWidget->hide();

    // Edit menu
    connect(m_ui->actionUndo, SIGNAL(triggered()), this, SLOT(Undo()));
    connect(m_ui->actionRedo, SIGNAL(triggered()), this, SLOT(Redo()));

    // Game Object menu
    connect(m_ui->actionCreate_Game_Object, SIGNAL(triggered()), this, SLOT(CreateGameObject()));
    connect(m_ui->actionDelete_Game_Object, SIGNAL(triggered()), this, SLOT(DeleteGameObject()));
}

MainEditorWindow::~MainEditorWindow()
{
    delete m_ui;
    delete m_glWidget;
}

void MainEditorWindow::Paint()
{
    m_glWidget->update();
}

void MainEditorWindow::SetHierarchyModel(HierarchyModel* model)
{
    m_model = model;
    m_view->setModel(m_model);
    DebugLog("Scene loaded!");
    connect(m_view->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(OnSelectionChanged(const QItemSelection &, const QItemSelection &)));
    m_view->expandAll();
}

void MainEditorWindow::DebugLog(string text)
{
    m_ui->textEdit_DebugOutput->append(QString(text.c_str()));
}

// TODO: Qt textboxes seem to capture ctrl + z and do their own undo. stop that!
// TODO: just selecting a game object counts as a bunch of update commands. fix that!
void MainEditorWindow::Undo()
{
    // TODO disable menu option if undo is not currently available
    bool success = m_commandManager.Undo();

    if (success)
    {
        DebugLog("Undo");
    }
    else
    {
        DebugLog("Can't undo. Stack is empty.");
    }
}

void MainEditorWindow::Redo()
{
    // TODO disable menu option if redo is not currently available
    bool success = m_commandManager.Redo();

    if (success)
    {
        DebugLog("Redo");
    }
    else
    {
        DebugLog("Can't redo. Stack is empty.");
    }
}

void MainEditorWindow::CreateGameObject()
{
    DebugLog("Creating game object");

    QModelIndex index = m_view->selectionModel()->currentIndex();
    HierarchyModel* model = (HierarchyModel*)(m_view->model());
    CreateGameObjectCommand* command = new CreateGameObjectCommand(model, index);
    m_commandManager.ExecuteCommand(command);
}

void MainEditorWindow::DeleteGameObject()
{
    DebugLog("Deleting game object");

    QModelIndex index = m_view->selectionModel()->currentIndex();
    HierarchyModel* model = (HierarchyModel*)(m_view->model());
    DeleteGameObjectCommand* command = new DeleteGameObjectCommand(model, index);
    m_commandManager.ExecuteCommand(command);
}

// TODO: discard trivial updates that don't actually modify values
void MainEditorWindow::UpdateGameObjectTransform(Vector3 vector, VectorType type)
{
    DebugLog("Updating gameobject transform");

    QModelIndex index = m_view->selectionModel()->currentIndex();
    HierarchyModel* model = (HierarchyModel*)(m_view->model());
    ModifyTransformCommand* command = new ModifyTransformCommand(model, index, vector, type);
    m_commandManager.ExecuteCommand(command);
}

void MainEditorWindow::OnSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{
    // TODO enforce selection of only one object at a time?

    // Get the selected game object
    const QModelIndex index = m_view->selectionModel()->currentIndex();
    GameObject* go = (GameObject*)index.internalPointer();

    if (go != NULL)
    {
        // Show the components for the selected game object
        m_transformWidget->SetGameObject(go);
        m_transformWidget->show();
    }
    else
    {
        // Nothing is selected, so hide widgets
        m_transformWidget->SetGameObject(NULL);
        m_transformWidget->hide();
    }
}

void MainEditorWindow::resizeEvent(QResizeEvent* resizeEvent)
{
    QSize newSize = resizeEvent->size();
    int width = newSize.width();
    int height = newSize.height();

    DebugLog("Window resize!");

    //TODO: scale/reposition widgets
}