#include "EditorCommands.h"
#include "GameObject.h"
#include "HierarchyModel.h"

namespace EditorCommands
{
    CreateGameObjectCommand::CreateGameObjectCommand(HierarchyModel* model, QModelIndex index)
    {
        m_model = model;
        m_index = index;
    }

    void CreateGameObjectCommand::Execute()
    {
        m_model->insertRow(0, m_index);
    }

    void CreateGameObjectCommand::Undo()
    {
        m_model->removeRow(0, m_index);
    }

    //-----------------------------------------------------------------------------------------------

    DeleteGameObjectCommand::DeleteGameObjectCommand(HierarchyModel* model, QModelIndex index)
    {
        m_model = model;
        m_index = index;

        // TODO handle null parent (deleting the root)
        m_gameObject = (GameObject*)m_index.internalPointer();
        m_parent = (GameObject*)m_index.parent().internalPointer();
        m_position = m_index.row();
    }

    void DeleteGameObjectCommand::Execute()
    {
        m_model->removeRow(m_index.row(), m_index.parent());
    }

    void DeleteGameObjectCommand::Undo()
    {
        m_model->insertChild(m_index.parent(), m_parent, m_gameObject, m_position);
    }

    //-----------------------------------------------------------------------------------------------

    RenameGameObjectCommand::RenameGameObjectCommand(HierarchyModel* model, QModelIndex index, string name)
    {
        m_model = model;
        m_index = index;
        m_name = name;
        m_previousName = m_model->getItem(index)->GetName();
    }

    void RenameGameObjectCommand::Execute()
    {
        m_model->setItemName(m_index, m_name);
    }

    void RenameGameObjectCommand::Undo()
    {
        m_model->setItemName(m_index, m_previousName);
    }

    //-----------------------------------------------------------------------------------------------

    ModifyTransformCommand::ModifyTransformCommand(HierarchyModel* model, QModelIndex index, Vector3 vector, VectorType type)
    {
        m_model = model;
        m_gameObject = (GameObject*)index.internalPointer();
        m_vector = vector;
        m_type = type;
    }

    void ModifyTransformCommand::Execute()
    {
        if (m_gameObject != NULL)
        {
            switch (m_type)
            {
            case eVector_Position:
                m_previousVector = m_gameObject->GetLocalTransform().GetPosition();
                m_gameObject->GetLocalTransform().SetPosition(m_vector);
                break;
            case eVector_Rotation:
                m_previousVector = m_gameObject->GetLocalTransform().GetRotation();
                m_gameObject->GetLocalTransform().SetRotation(m_vector);
                break;
            case eVector_Scale:
                m_previousVector = m_gameObject->GetLocalTransform().GetScale();
                m_gameObject->GetLocalTransform().SetScale(m_vector);
                break;
            }
        }
    }

    void ModifyTransformCommand::Undo()
    {
        if (m_gameObject != NULL)
        {
            switch (m_type)
            {
            case eVector_Position:
                m_gameObject->GetLocalTransform().SetPosition(m_previousVector);
                break;
            case eVector_Rotation:
                m_gameObject->GetLocalTransform().SetRotation(m_previousVector);
                break;
            case eVector_Scale:
                m_gameObject->GetLocalTransform().SetScale(m_previousVector);
                break;
            }
        }
    }
}