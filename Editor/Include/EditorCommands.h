#pragma once

#include "CommandManager.h"
#include "Math\Algebra.h"
#include <qabstractitemmodel.h>

class GameObject;
class HierarchyModel;

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
