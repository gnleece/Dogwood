#pragma once

#include <qtreeview>

class MainEditorWindow;

class HierarchyView : public QTreeView
{
    Q_OBJECT

public:
    HierarchyView(MainEditorWindow* window);

    void mouseReleaseEvent(QMouseEvent* event);
    void UpdateSelectedObject();

signals:
    void SelectedObjectChanged(QModelIndex& newIndex);

private:
    MainEditorWindow*   m_window;
    QModelIndex         m_currentIndex;
};