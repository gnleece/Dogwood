#pragma once

#include <QtWidgets>
#include "Math\Algebra.h"

class GameObject;
class MainEditorWindow;
class VectorWidget;

namespace Ui
{
    class TransformWidget;
}

class TransformWidget : public QWidget
{
    Q_OBJECT

public:
    TransformWidget(QWidget* parent = 0, MainEditorWindow* window = 0);

    void SetGameObject(GameObject* gameObject);
    void Refresh();

public slots:
    void UpdatePosition(Vector3& position);
    void UpdateRotation(Vector3& rotation);
    void UpdateScale(Vector3& scale);

private:
    Ui::TransformWidget*    m_ui;
    MainEditorWindow*       m_window;
    GameObject*             m_gameObject;

    VectorWidget*           m_positionWidget;
    VectorWidget*           m_rotationWidget;
    VectorWidget*           m_scaleWidget;
};
