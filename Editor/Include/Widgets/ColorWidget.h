#pragma once

#include <QtWidgets>

#include "Rendering\Colour.h"

namespace Ui
{
    class ColorWidget;
}

class ColorWidget : public QWidget
{
    Q_OBJECT

public:
    ColorWidget(QWidget* parent = 0);

private:
    Ui::ColorWidget*    m_ui;
    QColor              m_color;

private slots:
    void                OnButtonClick();
    void                SetButtonStyle();
    ColourRGB           QColorToColourRGB(QColor qcolor);
};