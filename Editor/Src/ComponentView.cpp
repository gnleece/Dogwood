#include "ComponentView.h"

#include <qevent.h>
#include <QHeaderView>

ComponentView::ComponentView(MainEditorWindow* window) :
    QTableView(), m_window(window)
{
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::DropOnly);
    verticalHeader()->setVisible(false);
    horizontalHeader()->setVisible(false);
}

void ComponentView::dragEnterEvent(QDragEnterEvent *event)
{
    // TODO validate event data
    event->acceptProposedAction();
}

void ComponentView::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}