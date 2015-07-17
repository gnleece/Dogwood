#pragma once

#include <qabstractitemmodel.h>
#include <QItemSelection>
#include <QMainWindow>
#include <QSignalMapper>
#include <string>
#include <unordered_map>

#include "CommandManager.h"
#include "EditorCommands.h"
#include "Math\Algebra.h"

class AssetWidget;
class ComponentWidget;
class ComponentModel;
class GameObject;
class HierarchyModel;
class HierarchyView;
class MeshWidget;
class QTreeView;
class Scene;
class SceneViewWidget;
class TransformWidget;

using std::string;
using std::unordered_map;

// TODO This class is pretty bloated and should be majorly refactored,
// it's become a catch-all for editor side functionality

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
    ComponentWidget*        m_componentWidget;
    SceneViewWidget*        m_sceneViewWidget;
    TransformWidget*        m_transformWidget;
    MeshWidget*             m_meshWidget;

    QSignalMapper*          m_addMeshSignalMapper;

    HierarchyView*          m_view;
    HierarchyModel*         m_model;
    Scene*                  m_scene;

    GameObject*             m_copiedGameObject;
    GameObject*             m_selectedGameObject;

public slots:
    void SaveProject();
    void RebuildComponentSchema();

private slots:
    void SetupComponentWidgets();
    void SetupMenuCommands();

    void NewProject();
    void OpenProject();
    void NewScene();
    void OpenScene();
    void UnloadScene();
    void SaveScene();
    void SaveSceneAs();
    void SaveAll();

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

    void AddMeshPrimitive(const QString& meshName);
    void AddGameComponent(unsigned int guid);

    void OnHierarchySelectionChanged(QModelIndex& newIndex);
    void SwitchSelectObject(GameObject* gameobject);

    void UpdateMenuState();
    void MapHookup(QSignalMapper* signalMapper, QObject* sender, QString text);
};