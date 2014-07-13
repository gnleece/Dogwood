#include "maineditorwindow.h"
#include "HierarchyModel.h"
#include "ui_maineditorwindow.h"
#include "GLWidget.h"

#include <QtWidgets>

MainEditorWindow::MainEditorWindow(QWidget *parent)
: m_ui(new Ui::MainEditorWindow)
{
    m_glWidget = new GLWidget;
    m_glWidget->setFixedSize(640, 480);
 
    m_ui->setupUi(this);
    m_ui->verticalLayout->addWidget(m_glWidget);

    setWindowTitle(tr("Dogwood Editor!"));
    m_ui->textEdit_DebugOutput->append("Loading scene...");

    connect(m_ui->actionCreate_Game_Object, SIGNAL(triggered()), this, SLOT(CreateGameObject()));
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
    m_ui->treeView->setModel(m_model);
    DebugLog("Scene loaded!");
}

void MainEditorWindow::DebugLog(string text)
{
    m_ui->textEdit_DebugOutput->append(QString(text.c_str()));
}

void MainEditorWindow::CreateGameObject()
{
    DebugLog("Creating game object");

    QModelIndex index = m_ui->treeView->selectionModel()->currentIndex();
    QAbstractItemModel *model = m_ui->treeView->model();

    if (!model->insertRow(index.row() + 1, index.parent()))
        return;

    QModelIndex child = model->index(index.row() + 1, 1, index.parent());
    model->setData(child, QVariant("GameObject"), Qt::EditRole);
}