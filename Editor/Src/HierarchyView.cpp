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

void HierarchyView::mousePressEvent(QMouseEvent *event)
{
    // If user clicked on nothing, and something was previously selected, unselect it
    if (!(selectionModel()->selectedIndexes().isEmpty()))
    {
        m_window->SelectObject(NULL);
    }
    
    QTreeView::mousePressEvent(event);
}