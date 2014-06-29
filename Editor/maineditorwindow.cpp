#include "maineditorwindow.h"
#include "ui_maineditorwindow.h"
#include "GLWidget.h"

#include <QtWidgets>

MainEditorWindow::MainEditorWindow(QWidget *parent)
: QMainWindow(parent),
  m_ui(new Ui::MainEditorWindow)
{
    m_glWidget = new GLWidget;
    m_glWidget->setFixedSize(640, 480);
 
    m_ui->setupUi(this);
    m_ui->verticalLayout->addWidget(m_glWidget);

    setWindowTitle(tr("Dogwood Editor!"));
}

MainEditorWindow::~MainEditorWindow()
{
    delete m_ui;
    delete m_glWidget;
}

void MainEditorWindow::SetRoot(GameObject* root)
{
    m_glWidget->SetRoot(root);
}