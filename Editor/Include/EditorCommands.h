#pragma once

///////////////////////////////////////////////////////////////////////
// Each command is a class that implements ICommand, so that it can 
// be managed by the CommandManager (an undo/redo stack). Any action
// that manipulates the game object hierarchy in any way should be
// implemented as a command in this way so that it can be undone/redone.
///////////////////////////////////////////////////////////////////////

#include "CommandManager.h"
#include "Rendering\Color.h"
#include "Math\Algebra.h"
#include "Math\Transform.h"

#include <qabstractitemmodel.h>
#include <QTime>
#include <string>

class ComponentWidget;
class GameObjectMimeData;
class HierarchyModel;
class Material;
class QTreeView;
class ToolsideGameObject;
class TransformWidget;

using std::string;

namespace EditorCommands
{
    class CreateGameObjectCommand : public ICommand
    {
    public:
        CreateGameObjectCommand(HierarchyModel* model, QTreeView* view, QModelIndex index);
        void Execute();
        void Undo();
    private:
        HierarchyModel* m_model;
        QTreeView*      m_view;
        QModelIndex     m_index;
        int             m_position;
    };

    class DeleteGameObjectCommand : public ICommand
    {
    public:
        DeleteGameObjectCommand(HierarchyModel* model, QTreeView* view, QModelIndex index);
        void Execute();
        void Undo();
    private:
        HierarchyModel*     m_model;
        QTreeView*          m_view;
        ToolsideGameObject* m_gameObject;
        int                 m_position;
        QModelIndex         m_index;
        ToolsideGameObject* m_parent;
    };

    class RenameGameObjectCommand : public ICommand
    {
    public:
        RenameGameObjectCommand(HierarchyModel* model, QModelIndex index, string name);
        void Execute();
        void Undo();
    private:
        HierarchyModel* m_model;
        QModelIndex     m_index;
        string          m_name;
        string          m_previousName;
    };

    class ReparentGameObjectCommand : public ICommand
    {
    public:
        ReparentGameObjectCommand(HierarchyModel* model, GameObjectMimeData* mimeData, const QModelIndex& newParentIndex);
        void Execute();
        void Undo();
    private:
        HierarchyModel*     m_model;
        GameObjectMimeData* m_mimeData;
        QModelIndex         m_newParentIndex;
        int                 m_position;
    };

    class PasteGameObjectCommand : public ICommand
    {
    public:
        PasteGameObjectCommand(HierarchyModel* model, QTreeView* view, QModelIndex index, ToolsideGameObject* gameObject);
        void Execute();
        void Undo();
    private:
        HierarchyModel*     m_model;
        QTreeView*          m_view;
        QModelIndex         m_index;
        ToolsideGameObject* m_gameObject;
        ToolsideGameObject* m_parentObject;
    };

    class ModifyTransformCommand : public ICommand
    {
    public:
        ModifyTransformCommand(ToolsideGameObject* gameObject, Vector3 vector, TransformVectorType type);
        void Execute();
        void Undo();
        bool Collapse(ICommand* command);

        static ComponentWidget* sComponentWidget;

    private:
        ToolsideGameObject* m_gameObject;
        TransformVectorType m_type;
        Vector3             m_vector;
        Vector3             m_previousVector;
        QTime               m_timestamp;

        const int           MaxCollapseTimeDelta = 100;
    };

    class ChangeMaterialColorCommand : public ICommand
    {
    public:
        ChangeMaterialColorCommand(Material* material, string name, ColorRGB color);
        void Execute();
        void Undo();

    private:
        Material*           m_material;
        string              m_name;
        ColorRGB            m_color;
        ColorRGB            m_previousColor;
    };
}
