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
#include "Physics\Collider.h"

class AssetWidget;
class ComponentWidget;
class ComponentModel;
class HierarchyModel;
class HierarchyView;
class QTreeView;
class Scene;
class SceneViewWidget;
class ToolsideGameObject;

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

    void                PostSetup();
    void                Update();

    void                SetHierarchyModel(HierarchyModel* model);

    void                SelectObject(ToolsideGameObject* gameObject);
    ToolsideGameObject* GetSelectedObject();

    bool                IsOpen();

    void                Refresh();

    virtual void        resizeEvent(QResizeEvent* resizeEvent);
    virtual void        closeEvent(QCloseEvent *event);

private:
    bool                    m_open;

    Ui::MainEditorWindow*   m_ui;
    AssetWidget*            m_assetWidget;
    ComponentWidget*        m_componentWidget;
    SceneViewWidget*        m_sceneViewWidget;

    QSignalMapper*          m_addMeshSignalMapper;
    QSignalMapper*          m_addPhysicsSignalMapper;

    HierarchyView*          m_view;
    HierarchyModel*         m_model;
    Scene*                  m_scene;

    ToolsideGameObject*     m_copiedGameObject;
    ToolsideGameObject*     m_selectedGameObject;

    QDialog*                m_settingsDialog;

public slots:
    void SaveProject();
    void RebuildComponentSchema();
    void RebuildShaderSchema();

private slots:
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

    void ShowProjectSettingsDialog();

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
    void AddCollider(int type);
    void AddParticleSystem();

    void OnHierarchySelectionChanged(QModelIndex& newIndex);
    void SwitchSelectObject(ToolsideGameObject* gameobject);

    void UpdateMenuState();

    void SignalMapHookup(QSignalMapper* signalMapper, QObject* sender, QString text);
    void SignalMapHookup(QSignalMapper* signalMapper, QObject* sender, int value);
};