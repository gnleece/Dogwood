#pragma once

#include <QtWidgets>
#include <string>

class ComponentModel;
class ComponentView;
class MainEditorWindow;
class ToolsideGameObject;

namespace Ui
{
    class ComponentWidget;
}

class ComponentWidget : public QWidget
{
    Q_OBJECT

public:
    ComponentWidget(QWidget* parent = 0, MainEditorWindow* window = NULL);

    void    Init(ToolsideGameObject* go);
    void    ReInit();
    void    Refresh();

private:
    Ui::ComponentWidget*    m_ui;
    ComponentModel*         m_sourceModel;
    ComponentView*          m_view;
    ToolsideGameObject*     m_gameObject;

private slots:
    void    OnDoubleClick(const QModelIndex& index);
    void    OnLayoutChanged();
};