#pragma once

#include <qabstractitemmodel.h>
#include <QItemSelection>
#include <QMainWindow>
#include <string>

#include "CommandManager.h"
#include "Math\Algebra.h"

class GLWidget;
class GameObject;
class HierarchyModel;
class QTreeView;
class TransformWidget;

using std::string;

enum VectorType { eVector_Position, eVector_Rotation, eVector_Scale };

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

    void UpdateGameObjectTransform(Vector3 vector, VectorType type);

private:
    Ui::MainEditorWindow*   m_ui;
    GLWidget*               m_glWidget;
    TransformWidget*        m_transformWidget;

    QTreeView*              m_view;
    HierarchyModel*         m_model;

    CommandManager          m_commandManager;

private slots:
    void CreateGameObject();
    void DeleteGameObject();
    void Undo();
    void Redo();
    void OnSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
};

// TODO clean this up
namespace EditorCommands
{
    class CreateGameObjectCommand : public ICommand
    {
    public:
        CreateGameObjectCommand(HierarchyModel* model, QModelIndex index);
        void Execute();
        void Undo();
    private:
        HierarchyModel* m_model;
        QModelIndex m_index;
    };

    class DeleteGameObjectCommand : public ICommand
    {
    public:
        DeleteGameObjectCommand(HierarchyModel* model, QModelIndex index);
        void Execute();
        void Undo();
    private:
        HierarchyModel* m_model;
        GameObject* m_gameObject;
        int m_position;
        QModelIndex m_index;
        GameObject* m_parent;
    };

    class ModifyTransformCommand : public ICommand
    {
    public:
        ModifyTransformCommand(HierarchyModel* model, QModelIndex index, Vector3 vector, VectorType type);
        void Execute();
        void Undo();

    private:
        VectorType m_type;
        Vector3 m_vector;
        Vector3 m_previousVector;
        HierarchyModel* m_model;
        GameObject* m_gameObject;
    };

}
