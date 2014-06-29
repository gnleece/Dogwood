#ifndef MAINEDITORWINDOW_H
#define MAINEDITORWINDOW_H

#include <QMainWindow>

#include "MyGLWidget.h"

namespace Ui {
class MainEditorWindow;
}

class MainEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainEditorWindow(QWidget *parent = 0);
    ~MainEditorWindow();

protected:
    MyGLWidget *glwidget;

private:
    Ui::MainEditorWindow *ui;
};

#endif // MAINEDITORWINDOW_H