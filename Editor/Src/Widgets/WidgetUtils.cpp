#include "Widgets\WidgetUtils.h"

#include <qframe.h>
#include <qwidget.h>

QFrame* AddLineSeparator(QWidget* parent)
{
    QFrame* line = new QFrame(parent);
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setFixedHeight(10);
    return line;
}