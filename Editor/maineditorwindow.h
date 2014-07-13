#pragma once

#include <QMainWindow>
#include <string>

class GLWidget;
class GameObject;
class HierarchyModel;
class QTreeView;

using std::string;

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
    void DebugLog(string text);

private:
    Ui::MainEditorWindow*   m_ui;
    GLWidget*               m_glWidget;

    QTreeView*              m_view;
    HierarchyModel*         m_model;

private slots:
    void CreateGameObject();
    void DeleteGameObject();
};
