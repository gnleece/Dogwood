#pragma once

#include <qtreeview>

class MainEditorWindow;

class ComponentView : public QTreeView
{
    Q_OBJECT

public:
    ComponentView(MainEditorWindow* window);

    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);

    void drawBranches(QPainter* painter, const QRect& rect, const QModelIndex& index) const;

private:
    MainEditorWindow*   m_window;
};