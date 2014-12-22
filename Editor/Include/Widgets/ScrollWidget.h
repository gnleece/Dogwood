#pragma once

#include <QtWidgets>

// TODO this whole widget is an ugly temp hack to allow scrolling through
// arbitrarily many components on a game object

namespace Ui
{
    class ScrollWidget;
}

class ScrollWidget : public QWidget
{
    Q_OBJECT

public:
    ScrollWidget(QWidget* parent = 0);
    void AddChildWidget(QWidget* child);

private:
    Ui::ScrollWidget*   m_ui;

    int                 m_height;
    int                 m_width;
    int                 m_numWidgets;
};