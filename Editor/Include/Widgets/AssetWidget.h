#pragma once

#include <QtWidgets>

class AssetDatabaseModel;

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
    Ui::AssetWidget*    m_ui;

    AssetDatabaseModel* m_model;
};