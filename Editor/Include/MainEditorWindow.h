#pragma once

#include <qabstractitemmodel.h>
#include <QItemSelection>
#include <QMainWindow>
#include <string>

#include "CommandManager.h"
#include "EditorCommands.h"
#include "Math\Algebra.h"

class GameObject;
class GameProject;
class HierarchyModel;
class QTreeView;
class Scene;
class SceneViewWidget;
class TransformWidget;

using std::string;

namespace Ui
{
    class MainEditorWindow;
}

class MainEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainEditorWindow(QWidget* parent = 0);
    ~MainEditorWindow();
    void PostSetup();

    void Paint();
    virtual void resizeEvent(QResizeEvent* resizeEvent);

    void SetHierarchyModel(HierarchyModel* model);
    void UpdateGameObjectTransform(Vector3 vector, VectorType type);

private:
    Ui::MainEditorWindow*   m_ui;
    SceneViewWidget*        m_sceneViewWidget;
    TransformWidget*        m_transformWidget;

    QTreeView*              m_view;
    HierarchyModel*         m_model;
    GameProject*            m_project;
    Scene*                  m_scene;

    GameObject*             m_copiedGameObject;
    GameObject*             m_selectedGameObject;

private slots:
    void NewScene();
    void OpenScene();
    void SaveScene();
    void SaveSceneAs();

    void Undo();
    void Redo();

    void CreateGameObject();
    void DeleteGameObject();
    void CopyGameObject();
    void CutGameObject();
    void PasteGameObject();

    void OpenTestProject();       // temp shortcut for debugging

    void OnSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
};