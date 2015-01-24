#pragma once

///////////////////////////////////////////////////////////////////////
// Each command is a class that implements ICommand, so that it can 
// be managed by the CommandManager (an undo/redo stack). Any action
// that manipulates the game object hierarchy in any way should be
// implemented as a command in this way so that it can be undone/redone.
///////////////////////////////////////////////////////////////////////

#include "CommandManager.h"
#include "Rendering\Colour.h"
#include "Math\Algebra.h"
#include <qabstractitemmodel.h>
#include <QTime>
#include <string>

class GameObject;
class GameObjectMimeData;
class HierarchyModel;
class Material;
class QTreeView;
class TransformWidget;

using std::string;

enum VectorType { eVector_Position, eVector_Rotation, eVector_Scale };
enum MaterialColorType { eMaterial_Diffuse, eMaterial_Ambient, eMaterial_Specular };

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
        HierarchyModel* m_model;
        QTreeView*      m_view;
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
        int                 m_position;
    };

    class PasteGameObjectCommand : public ICommand
    {
    public:
        PasteGameObjectCommand(HierarchyModel* model, QTreeView* view, QModelIndex index, GameObject* gameObject);
        void Execute();
        void Undo();
    private:
        HierarchyModel* m_model;
        QTreeView*      m_view;
        QModelIndex     m_index;
        GameObject*     m_gameObject;
        GameObject*     m_parentObject;
    };

    class ModifyTransformCommand : public ICommand
    {
    public:
        ModifyTransformCommand(GameObject* gameObject, Vector3 vector, VectorType type);
        void Execute();
        void Undo();
        bool Collapse(ICommand* command);

    private:
        GameObject*         m_gameObject;
        VectorType          m_type;
        Vector3             m_vector;
        Vector3             m_previousVector;
        QTime               m_timestamp;

        const int           MaxCollapseTimeDelta = 100;
    };

    class ChangeMaterialColorCommand : public ICommand
    {
    public:
        ChangeMaterialColorCommand(Material* material, MaterialColorType type, ColourRGB color);
        void Execute();
        void Undo();

    private:
        Material*           m_material;
        MaterialColorType   m_type;
        ColourRGB           m_color;
        ColourRGB           m_previousColor;
    };
}
