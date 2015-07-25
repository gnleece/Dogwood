#pragma once

#include "Rendering\Colour.h"
#include <QColor>

ColourRGB    QColorToColourRGB(QColor qcolor);
QColor       ColourRGBToQColor(ColourRGB color);