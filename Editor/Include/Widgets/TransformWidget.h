#pragma once

#include <QtWidgets>
#include "Math\Algebra.h"

class VectorEdit;

namespace Ui
{
    class TransformWidget;
}

class TransformWidget : public QWidget
{
    Q_OBJECT

public:
    TransformWidget(QWidget* parent = 0);

public slots:
    void UpdatePosition(Vector3& position);
    void UpdateRotation(Vector3& rotation);
    void UpdateScale(Vector3& scale);

private:
    Ui::TransformWidget*    m_ui;

    VectorEdit*             m_positionWidget;
    VectorEdit*             m_rotationWidget;
    VectorEdit*             m_scaleWidget;
};
