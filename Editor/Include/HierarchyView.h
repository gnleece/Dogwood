#pragma once

#include <qtreeview.h>

class MainEditorWindow;

class HierarchyView : public QTreeView
{
    Q_OBJECT

public:
    HierarchyView(MainEditorWindow* window);

    void mousePressEvent(QMouseEvent *event);

private:
    MainEditorWindow*   m_window;
};