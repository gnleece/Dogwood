#include "Widgets\AssetWidget.h"
#include "AssetDatabaseModel.h"

#include "..\GeneratedFiles\ui_assetwidget.h"

AssetWidget::AssetWidget(QWidget* parent)
: m_ui(new Ui::AssetWidget)
{
    m_ui->setupUi(this);

    m_model = new AssetDatabaseModel(this);
    m_ui->meshTableView->setModel(m_model);
    m_ui->meshTableView->show();
}