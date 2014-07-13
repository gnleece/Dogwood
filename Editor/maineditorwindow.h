#pragma once

#include <QMainWindow>

class GLWidget;
class GameObject;
class HierarchyModel;

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

    void Paint();

    void SetHierarchyModel(HierarchyModel* model);

private:
    Ui::MainEditorWindow*   m_ui;
    GLWidget*               m_glWidget;
};
