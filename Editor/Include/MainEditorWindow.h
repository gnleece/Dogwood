#pragma once

#include <qabstractitemmodel.h>
#include <QMainWindow>
#include <string>

#include "CommandManager.h"

class GLWidget;
class GameObject;
class HierarchyModel;
class QTreeView;
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
    explicit MainEditorWindow(QWidget *parent = 0);
    ~MainEditorWindow();

    void Paint();

    void SetHierarchyModel(HierarchyModel* model);
    void DebugLog(string text);

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
}
