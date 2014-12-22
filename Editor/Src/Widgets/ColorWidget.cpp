#include "Widgets\ColorWidget.h"

#include "..\GeneratedFiles\ui_colorwidget.h"

#include <cstdio>
#include <iostream>
#include <string>
#include <QColorDialog>

using std::string;

ColorWidget::ColorWidget(QWidget* parent)
: m_ui(new Ui::ColorWidget)
{
    m_ui->setupUi(this);
    connect(m_ui->button, SIGNAL(clicked()), this, SLOT(OnButtonClick()));

    SetButtonStyle();
}

void ColorWidget::OnButtonClick()
{
    // Open a ColorDialog to let the user pick a color
    m_color = QColorDialog::getColor();    
    SetButtonStyle();
}

void ColorWidget::SetButtonStyle()
{
    string styleTemplate = "border-style: solid; border-color: black; border-width: 1px; background-color: %s;";
    
    // Set the background color of the button to match the current color value
    char stylebuf[100];
    sprintf(stylebuf, styleTemplate.c_str(), qPrintable(m_color.name()));
    m_ui->button->setStyleSheet(stylebuf);
}

ColourRGB ColorWidget::QColorToColourRGB(QColor qcolor)
{
    // TODO implement me
    return ColourRGB::Black;
}