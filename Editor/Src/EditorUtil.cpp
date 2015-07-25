#include "EditorUtil.h"

ColourRGB QColorToColourRGB(QColor qcolor)
{
    ColourRGB ret;
    ret.r = qcolor.redF();
    ret.g = qcolor.greenF();
    ret.b = qcolor.blueF();
    return ret;
}

QColor ColourRGBToQColor(ColourRGB color)
{
    QColor ret;
    ret.setRedF(color.r);
    ret.setGreenF(color.g);
    ret.setBlueF(color.b);
    return ret;
}