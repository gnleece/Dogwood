#pragma once

#include <QtWidgets>

namespace Ui
{
    class MeshWidget;
}

class MeshWidget : public QWidget
{
    Q_OBJECT

public:
    MeshWidget(QWidget* parent = 0);

private:
    Ui::MeshWidget*    m_ui;
};