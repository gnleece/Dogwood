#include "ComponentView.h"

#include <qevent.h>
#include <QHeaderView>
#include <QPainter>

ComponentView::ComponentView(MainEditorWindow* window) :
  QTreeView(), m_window(window)
{
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::DropOnly);
    setHeaderHidden(true);
    setIndentation(10);
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

void ComponentView::drawBranches(QPainter* painter, const QRect& rect, const QModelIndex& index) const
{
    QColor color(230, 230, 230);
    painter->fillRect(rect, color);
    QTreeView::drawBranches(painter, rect, index);
}