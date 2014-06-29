#pragma once

#include <QMainWindow>

class GLWidget;

namespace Ui
{
    class MainEditorWindow;
}

class MainEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainEditorWindow(QWidget *parent = 0);
    ~MainEditorWindow();

private:
    Ui::MainEditorWindow*   m_ui;
    GLWidget*               m_glWidget;
};
