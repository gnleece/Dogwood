#include "EditorCommands.h"
#include "GameObject.h"
#include "GameObjectMimeData.h"
#include "HierarchyModel.h"
#include "Rendering\Material.h"

#include <QtWidgets>

namespace EditorCommands
{
    CreateGameObjectCommand::CreateGameObjectCommand(HierarchyModel* model, QTreeView* view, QModelIndex index)
    {
        m_model = model;
        m_index = index.isValid() ? index : QModelIndex();
        m_view = view;
        m_position = m_model->rowCount(m_index);
    }

    void CreateGameObjectCommand::Execute()
    {
        m_model->insertRow(m_position, m_index);
        m_view->setExpanded(m_index, true);
    }

    void CreateGameObjectCommand::Undo()
    {
        m_model->removeRow(m_position, m_index);
    }

    //-----------------------------------------------------------------------------------------------

    DeleteGameObjectCommand::DeleteGameObjectCommand(HierarchyModel* model, QTreeView* view, QModelIndex index)
    {
        m_model = model;
        m_view = view;
        m_index = index;

        // TODO handle null parent (deleting the root)
        m_gameObject = m_model->getItem(m_index);
        m_parent = m_model->getItem(m_index.parent());
        m_position = m_index.row();
    }

    void DeleteGameObjectCommand::Execute()
    {
        m_model->removeRow(m_index.row(), m_index.parent());
    }

    void DeleteGameObjectCommand::Undo()
    {
        m_model->insertChild(m_index.parent(), m_parent, m_gameObject, m_position);
        m_view->setExpanded(m_index.parent(), true);
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

    ReparentGameObjectCommand::ReparentGameObjectCommand(HierarchyModel* model, GameObjectMimeData* mimeData, const QModelIndex& newParentIndex)
    {
        m_model = model;
        m_mimeData = mimeData;
        m_newParentIndex = newParentIndex.isValid() ? newParentIndex : QModelIndex();
        m_position = m_model->rowCount(m_newParentIndex);
    }

    void ReparentGameObjectCommand::Execute()
    {
        // Remove the original version of the child. Removal must be done first or Qt gets confused
        m_model->removeRow(m_mimeData->getOriginalRow(), m_mimeData->getOriginalParentIndex());

        // Add the child to the new parent (at the end of the child list)
        m_model->insertChild(m_newParentIndex,
                             m_model->getItem(m_newParentIndex),
                             m_mimeData->getGameObject(),
                             m_position);

        // TODO make auto-expand work here
    }

    void ReparentGameObjectCommand::Undo()
    {
        // Remove the object from the new parent. Removal must be done first or Qt gets confused
        m_model->removeRow(m_position, m_newParentIndex);

        // Add the object back to the old parent
        m_model->insertChild(m_mimeData->getOriginalParentIndex(),
                             m_model->getItem(m_mimeData->getOriginalParentIndex()), 
                             m_mimeData->getGameObject(),
                             m_mimeData->getOriginalRow());

        // TODO make auto-expand work here
    }

    //-----------------------------------------------------------------------------------------------

    PasteGameObjectCommand::PasteGameObjectCommand(HierarchyModel* model, QTreeView* view, QModelIndex index, GameObject* gameObject)
    {
        m_model = model;
        m_view = view;
        m_index = index;
        m_gameObject = gameObject->DeepCopy();
        m_parentObject = m_model->getItem(m_index);
    }

    void PasteGameObjectCommand::Execute()
    {
        m_model->insertChild(m_index, m_parentObject, m_gameObject, 0);
        m_view->setExpanded(m_index, true);
    }

    void PasteGameObjectCommand::Undo()
    {
        m_model->removeRow(0, m_index);
    }

    //-----------------------------------------------------------------------------------------------

    ModifyTransformCommand::ModifyTransformCommand(GameObject* gameObject, Vector3 vector, VectorType type)
    {
        m_gameObject = gameObject;
        m_vector = vector;
        m_type = type;
        m_timestamp = QTime::currentTime();
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

            // TODO need to emit a "transform changed" signal here, or in transform set function
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

            // TODO need to emit a "transform changed" signal here, or in transform set function
        }
    }

    bool ModifyTransformCommand::Collapse(ICommand* command)
    {
        // Check whether the new command is actually a ModifyTransformCommand
        ModifyTransformCommand* transformCommand = dynamic_cast<ModifyTransformCommand*> (command);
        if (transformCommand == NULL)
            return false;

        // Check whether the new command is the same type
        if (transformCommand->m_type != m_type)
            return false;

        // Check whether the command applies to the same gameobject
        if (transformCommand->m_gameObject != m_gameObject)
            return false;

        // Check whether the timestamps are close enough
        QTime currentTime = QTime::currentTime();
        int deltaTime = m_timestamp.msecsTo(transformCommand->m_timestamp);
        if (deltaTime > MaxCollapseTimeDelta)
            return false;

        // Success! Collapse the new command into this one
        m_vector = transformCommand->m_vector;
        m_timestamp = transformCommand->m_timestamp;
        return true;
    }

    //-----------------------------------------------------------------------------------------------

    ChangeMaterialColorCommand::ChangeMaterialColorCommand(Material* material, MaterialColorType type, ColourRGB color)
    {
        m_material = material;
        m_type = type;
        m_color = color;
    }

    void ChangeMaterialColorCommand::Execute()
    {
        m_previousColor = m_material->GetColour((Material::eMatColourType)m_type);
        m_material->SetColour((Material::eMatColourType)m_type, m_color);
    }

    void ChangeMaterialColorCommand::Undo()
    {
        m_material->SetColour((Material::eMatColourType)m_type, m_previousColor);
    }
}