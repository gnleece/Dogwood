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

ColourRGB ColorWidget::GetColor()
{
    return QColorToColourRGB(m_color);
}

void ColorWidget::SetColor(ColourRGB color)
{
    SetColor(ColourRGBToQColor(color));
}

void ColorWidget::SetColor(QColor color)
{
    m_color = color;
    SetButtonStyle();
}

void ColorWidget::OnButtonClick()
{
    // Open a ColorDialog to let the user pick a color
    m_color = QColorDialog::getColor();    
    SetButtonStyle();
    emit ColorChanged(QColorToColourRGB(m_color));
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
    ColourRGB ret;
    ret.r = qcolor.redF();
    ret.g = qcolor.greenF();
    ret.b = qcolor.blueF();
    return ret;
}

QColor ColorWidget::ColourRGBToQColor(ColourRGB color)
{
    QColor ret;
    ret.setRedF(color.r);
    ret.setGreenF(color.g);
    ret.setBlueF(color.b);
    return ret;
}