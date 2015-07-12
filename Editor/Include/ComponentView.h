#pragma once

#include <QTableView>

class MainEditorWindow;

class ComponentView : public QTableView
{
    Q_OBJECT

public:
    ComponentView(MainEditorWindow* window);

    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);

private:
    MainEditorWindow*   m_window;
};