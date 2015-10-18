#include "EditorUtil.h"

ColorRGB QColorToColorRGB(QColor qcolor)
{
    ColorRGB ret;
    ret.r = qcolor.redF();
    ret.g = qcolor.greenF();
    ret.b = qcolor.blueF();
    return ret;
}

QColor ColorRGBToQColor(ColorRGB color)
{
    QColor ret;
    ret.setRedF(color.r);
    ret.setGreenF(color.g);
    ret.setBlueF(color.b);
    return ret;
}