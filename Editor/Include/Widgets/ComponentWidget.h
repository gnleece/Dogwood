#pragma once

#include <QtWidgets>
#include <string>

class ComponentModel;
class ComponentView;
class GameObject;
class MainEditorWindow;

namespace Ui
{
    class ComponentWidget;
}

class ComponentWidget : public QWidget
{
    Q_OBJECT

public:
    ComponentWidget(QWidget* parent = 0, MainEditorWindow* window = NULL);

    void    Init(GameObject* go);
    void    Refresh();

private:
    Ui::ComponentWidget*    m_ui;
    ComponentModel*         m_sourceModel;
    ComponentView*          m_view;

private slots:
    void    OnDoubleClick(const QModelIndex& index);
};