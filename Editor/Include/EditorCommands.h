#pragma once

///////////////////////////////////////////////////////////////////////
// Each command is a class that implements ICommand, so that it can 
// be managed by the CommandManager (an undo/redo stack). Any action
// that manipulatse the game object hierarchy in any way should be
// implemented as a command in this way so that it can be undone/redone.
///////////////////////////////////////////////////////////////////////

#include "CommandManager.h"
#include "Math\Algebra.h"
#include <qabstractitemmodel.h>
#include <string>

class GameObject;
class HierarchyModel;
class GameObjectMimeData;

using std::string;

enum VectorType { eVector_Position, eVector_Rotation, eVector_Scale };

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
        QModelIndex     m_index;
    };

    class DeleteGameObjectCommand : public ICommand
    {
    public:
        DeleteGameObjectCommand(HierarchyModel* model, QModelIndex index);
        void Execute();
        void Undo();
    private:
        HierarchyModel* m_model;
        GameObject*     m_gameObject;
        int             m_position;
        QModelIndex     m_index;
        GameObject*     m_parent;
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
    };

    class PasteGameObjectCommand : public ICommand
    {
    public:
        PasteGameObjectCommand(HierarchyModel* model, QModelIndex index, GameObject* gameObject);
        void Execute();
        void Undo();
    private:
        HierarchyModel* m_model;
        QModelIndex     m_index;
        GameObject*     m_gameObject;
        GameObject*     m_parentObject;
    };

    class ModifyTransformCommand : public ICommand
    {
    public:
        ModifyTransformCommand(HierarchyModel* model, QModelIndex index, Vector3 vector, VectorType type);
        void Execute();
        void Undo();

    private:
        HierarchyModel* m_model;
        VectorType      m_type;
        Vector3         m_vector;
        Vector3         m_previousVector;
        GameObject*     m_gameObject;
    };
}
