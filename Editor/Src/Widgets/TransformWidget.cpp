#include "Widgets\TransformWidget.h"
#include "Widgets\VectorEdit.h"

#include "GameObject.h"

#include "..\GeneratedFiles\ui_transformwidget.h"

TransformWidget::TransformWidget(QWidget* parent)
: m_ui(new Ui::TransformWidget)
{
    m_ui->setupUi(this);

    // Create vector edit widgets
    m_positionWidget = new VectorEdit();
    m_positionWidget->SetTitle("Position");
    m_ui->verticalLayout->addWidget(m_positionWidget);

    m_rotationWidget = new VectorEdit();
    m_rotationWidget->SetTitle("Rotation");
    m_ui->verticalLayout->addWidget(m_rotationWidget);

    m_scaleWidget = new VectorEdit();
    m_scaleWidget->SetTitle("Scale");
    m_ui->verticalLayout->addWidget(m_scaleWidget);

    // Connect to vector edit signals
    connect(m_positionWidget, SIGNAL(VectorChanged(Vector3&)), this, SLOT(UpdatePosition(Vector3&)));
    connect(m_rotationWidget, SIGNAL(VectorChanged(Vector3&)), this, SLOT(UpdateRotation(Vector3&)));
    connect(m_scaleWidget, SIGNAL(VectorChanged(Vector3&)), this, SLOT(UpdateScale(Vector3&)));
}

void TransformWidget::SetValues(GameObject* gameObject)
{
    if (gameObject != NULL)
    {
        m_positionWidget->SetVector(gameObject->GetLocalTransform().GetPosition());
        m_rotationWidget->SetVector(gameObject->GetLocalTransform().GetRotation());
        m_scaleWidget->SetVector(gameObject->GetLocalTransform().GetScale());
    }
}

void TransformWidget::UpdatePosition(Vector3& position)
{
    // TODO implement me
}

void TransformWidget::UpdateRotation(Vector3& rotation)
{
    // TODO implement me
}

void TransformWidget::UpdateScale(Vector3& scale)
{
    // TODO implement me
}