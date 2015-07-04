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