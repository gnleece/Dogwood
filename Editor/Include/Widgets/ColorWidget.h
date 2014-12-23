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

    ColourRGB           GetColor();
    void                SetColor(ColourRGB color);
    void                SetColor(QColor color);

    static ColourRGB    QColorToColourRGB(QColor qcolor);
    static QColor       ColourRGBToQColor(ColourRGB color);

signals:
    void                ColorChanged(ColourRGB& color);

private:
    Ui::ColorWidget*    m_ui;
    QColor              m_color;

private slots:
    void                OnButtonClick();
    void                SetButtonStyle();
};