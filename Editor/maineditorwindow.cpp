#include "maineditorwindow.h"
#include "ui_maineditorwindow.h"

MainEditorWindow::MainEditorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainEditorWindow)
{
    ui->setupUi(this);

    glwidget = new MyGLWidget(this);

    // using the this pointer to emphase the location of the
    // member variable used.
    // NOTE: In the UI we defined a layout names verticalLayout
    ui->verticalLayout->addWidget(glwidget);
}

MainEditorWindow::~MainEditorWindow()
{
    delete ui;
}