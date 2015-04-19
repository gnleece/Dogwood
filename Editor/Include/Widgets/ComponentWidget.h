#pragma once

#include <QtWidgets>
#include <string>

class ComponentModel;
class GameObject;

namespace Ui
{
    class ComponentWidget;
}

class ComponentWidget : public QWidget
{
    Q_OBJECT

public:
    ComponentWidget(QWidget* parent = 0);

    void Init(GameObject* go);

private:
    Ui::ComponentWidget*    m_ui;
    ComponentModel*         m_sourceModel;
};