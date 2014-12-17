#include "Widgets\ScrollWidget.h"

#include "..\GeneratedFiles\ui_scrollwidget.h"

// TODO this whole widget is an ugly temp hack to allow scrolling through
// arbitrarily many components on a game object

ScrollWidget::ScrollWidget(QWidget* parent)
: QWidget(parent),
  m_ui(new Ui::ScrollWidget)
{ 
    m_ui->setupUi(this);
    m_height = 0;
    m_width = 0;
}

void ScrollWidget::AddChildWidget(QWidget* child)
{
    // TODO this is super hacky and doesn't account for removing children

    m_ui->verticalLayout->addWidget(child);
    m_height += child->height();
    m_width = std::max(m_width, child->width());
    
    setFixedHeight(m_height + 10);
    setFixedWidth(m_width + 15);
    m_ui->verticalGroupBox->setFixedHeight(m_height + 10);
    m_ui->verticalGroupBox->setFixedWidth(m_width + 15);
}