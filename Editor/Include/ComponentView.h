#pragma once

#include <QTableView>

class MainEditorWindow;

class ComponentView : public QTableView
{
    Q_OBJECT

public:
    ComponentView(MainEditorWindow* window);

private:
    MainEditorWindow*   m_window;
};