#include "HierarchyView.h"
#include "DebugLogger.h"
#include "MainEditorWindow.h"

HierarchyView::HierarchyView(MainEditorWindow* window) : 
    QTreeView(), m_window(window)
{
    setAcceptDrops(true);
    setDragEnabled(true);
    setRootIsDecorated(true);
    setHeaderHidden(true);
    setDefaultDropAction(Qt::DropAction::CopyAction);
    setDragDropMode(QAbstractItemView::DragDropMode::DragDrop);
    setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
}

void HierarchyView::mouseReleaseEvent(QMouseEvent*)
{
    DebugLogger::Singleton().Log("Hierarchy view mouse release");

    // We only update selection here on mouse *release*, not mouse press, because
    // we don't want to actually change selection on drag & drop
    UpdateSelectedObject();
}

void HierarchyView::UpdateSelectedObject()
{
    // Get the selected game object
    QModelIndex index;
    if (!(selectionModel()->selectedIndexes().isEmpty()))
    {
        index = selectionModel()->selectedIndexes().first();
    }

    if (index != m_currentIndex)
    {
        emit SelectedObjectChanged(index);
    }

    m_currentIndex = index;
}