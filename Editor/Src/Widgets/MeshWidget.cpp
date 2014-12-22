#include "Widgets\MeshWidget.h"

#include "Widgets\ColorWidget.h"
#include "..\GeneratedFiles\ui_meshwidget.h"

#include <string>

MeshWidget::MeshWidget(QWidget* parent)
: m_ui(new Ui::MeshWidget)
{
    m_ui->setupUi(this);
    setFixedHeight(240);

    for (int i = 0; i < 3; i++)
    {
        m_colorWidgets[i] = new ColorWidget(this);
    }

    m_ui->colorDiffuseLayout->addWidget(m_colorWidgets[0]);
    m_ui->colorAmbientLayout->addWidget(m_colorWidgets[1]);
    m_ui->colorSpecularLayout->addWidget(m_colorWidgets[2]);
}