#include "ComponentView.h"

#include "ComponentModel.h"
#include "ComponentModelItem.h"
#include <qevent.h>
#include <QHeaderView>
#include <QMenu>
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

void ComponentView::SetModel(ComponentModel* model)
{
    m_model = model;
    setModel(model);
}

void ComponentView::mousePressEvent(QMouseEvent* event)
{
    QTreeView::mousePressEvent(event);

    QPoint globalPos = event->globalPos();
    QModelIndex& index = indexAt(viewport()->mapFromGlobal(globalPos));

    ComponentModelItem* item = m_model->GetItem(index);
    ShowContextMenu(item, globalPos);
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

void ComponentView::ShowContextMenu(ComponentModelItem* item, QPoint globalPos)
{
    if (item == NULL)
        return;

    QMenu myMenu;
    MenuOptions options = item->GetMenuOptions();

    if (options != ComponentModelItem::CONTEXTMENU_NONE)
    {
        // Add each option as a menu action
        if ((options & ComponentModelItem::CONTEXTMENU_COPY) == ComponentModelItem::CONTEXTMENU_COPY)
        {
            myMenu.addAction("Copy");
        }
        if ((options & ComponentModelItem::CONTEXTMENU_DELETE) == ComponentModelItem::CONTEXTMENU_DELETE)
        {
            myMenu.addAction("Delete");
        }

        // Get the selection item, then handle it
        QAction* selectedItem = myMenu.exec(globalPos);
        if (selectedItem)
        {
            // TODO these string comparisons are pretty awful
            string selectedString = selectedItem->text().toStdString();

            if (selectedString.compare("Copy") == 0)
            {
                item->HandleMenuSelection(ComponentModelItem::CONTEXTMENU_COPY);
            }
            else if (selectedString.compare("Delete") == 0)
            {
                item->HandleMenuSelection(ComponentModelItem::CONTEXTMENU_DELETE);
            }
        }
    }
}