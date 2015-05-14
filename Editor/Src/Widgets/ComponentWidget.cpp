#include "Widgets\ComponentWidget.h"
#include "ComponentModel.h"

#include "..\GeneratedFiles\ui_componentwidget.h"

ComponentWidget::ComponentWidget(QWidget* parent) : m_ui(new Ui::ComponentWidget), m_sourceModel(NULL)
{
    m_ui->setupUi(this);

    QHeaderView *verticalHeader = m_ui->tableView->verticalHeader();
    verticalHeader->sectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(24);
}

void ComponentWidget::Init(GameObject* go)
{
    // Clear old model
    if (m_sourceModel != NULL)
    {
        delete m_sourceModel;
    }

    // Prepare new model
    m_sourceModel = new ComponentModel(this, go);
    m_ui->tableView->setModel(m_sourceModel);
    m_ui->tableView->show();
}