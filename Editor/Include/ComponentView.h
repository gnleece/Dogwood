#pragma once

#include <qtreeview>

class ComponentModel;
class ComponentModelItem;
class MainEditorWindow;

class ComponentView : public QTreeView
{
    Q_OBJECT

public:

    ComponentView(MainEditorWindow* window);

    void    SetModel(ComponentModel* model);

    void    mousePressEvent(QMouseEvent* event);
    void    dragEnterEvent(QDragEnterEvent *event);
    void    dragMoveEvent(QDragMoveEvent *event);

    void    drawBranches(QPainter* painter, const QRect& rect, const QModelIndex& index) const;

private:
    void    ShowContextMenu(ComponentModelItem* item, QPoint globalPos);

    MainEditorWindow*   m_window;
    ComponentModel*     m_model;
};