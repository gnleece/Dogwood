#pragma once

#include <QtWidgets>

namespace Ui
{
    class AssetWidget;
}

class AssetWidget : public QWidget
{
    Q_OBJECT

public:
    AssetWidget(QWidget* parent = 0);

private:
    Ui::AssetWidget*     m_ui;
};