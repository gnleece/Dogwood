#pragma once

#include "Rendering\Color.h"
#include <QColor>

ColorRGB    QColorToColorRGB(QColor qcolor);
QColor      ColorRGBToQColor(ColorRGB color);
