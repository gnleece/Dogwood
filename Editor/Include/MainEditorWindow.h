#pragma once

#include <qabstractitemmodel.h>
#include <QItemSelection>
#include <QMainWindow>
#include <string>

#include "CommandManager.h"
#include "EditorCommands.h"
#include "Math\Algebra.h"

class AssetWidget;
class GameObject;
class HierarchyModel;
class MeshWidget;
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

    void        PostSetup();
    void        Update();

    void        SetHierarchyModel(HierarchyModel* model);

    void        SelectObject(GameObject* gameObject);
    GameObject* GetSelectedObject();

    bool        IsOpen();

    virtual void resizeEvent(QResizeEvent* resizeEvent);
    virtual void closeEvent(QCloseEvent *event);

private:
    bool                    m_open;

    Ui::MainEditorWindow*   m_ui;
    AssetWidget*            m_assetWidget;
    SceneViewWidget*        m_sceneViewWidget;
    TransformWidget*        m_transformWidget;
    MeshWidget*             m_meshWidget;

    QTreeView*              m_view;
    HierarchyModel*         m_model;
    Scene*                  m_scene;

    GameObject*             m_copiedGameObject;
    GameObject*             m_selectedGameObject;

private slots:
    void SetupComponentWidgets();
    void SetupMenuCommands();

    void NewProject();
    void OpenProject();
    void SaveProject();
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

    void TransformTranslateButton();
    void TransformRotateButton();
    void TransformScaleButton();

    void OnSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
    void SwitchSelectObject(GameObject* gameobject);

    void UpdateMenuState();
};