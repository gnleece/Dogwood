#include "Widgets\TransformWidget.h"
#include "Widgets\VectorWidget.h"
#include "MainEditorWindow.h"

#include "GameObject.h"

#include "..\GeneratedFiles\ui_transformwidget.h"

using namespace EditorCommands;

TransformWidget::TransformWidget(QWidget* parent, MainEditorWindow* window)
: m_ui(new Ui::TransformWidget), m_window(window), m_gameObject(NULL)
{
    m_ui->setupUi(this);

    // Create vector widgets
    m_positionWidget = new VectorWidget(this);
    m_positionWidget->SetTitle("Position");
    m_ui->verticalLayout->addWidget(m_positionWidget);

    m_rotationWidget = new VectorWidget(this);
    m_rotationWidget->SetTitle("Rotation");
    m_ui->verticalLayout->addWidget(m_rotationWidget);

    m_scaleWidget = new VectorWidget(this);
    m_scaleWidget->SetTitle("Scale");
    m_ui->verticalLayout->addWidget(m_scaleWidget);

    // Connect to vector edit signals
    connect(m_positionWidget, SIGNAL(VectorChanged(Vector3&)), this, SLOT(UpdatePosition(Vector3&)));
    connect(m_rotationWidget, SIGNAL(VectorChanged(Vector3&)), this, SLOT(UpdateRotation(Vector3&)));
    connect(m_scaleWidget,    SIGNAL(VectorChanged(Vector3&)), this, SLOT(UpdateScale(Vector3&)));
}

void TransformWidget::SetGameObject(GameObject* gameObject)
{
    m_gameObject = gameObject;
    Refresh();
}

void TransformWidget::Refresh()
{
    if (m_gameObject != NULL)
    {
        // Display the position, rotation, and scale values of this game object
        m_positionWidget->SetVector(m_gameObject->GetLocalTransform().GetPosition());
        m_rotationWidget->SetVector(m_gameObject->GetLocalTransform().GetRotation());
        m_scaleWidget->SetVector(m_gameObject->GetLocalTransform().GetScale());
    }
}

void TransformWidget::UpdatePosition(Vector3& position)
{
    ExecuteModifyTransform(position, eVector_Position);
}

void TransformWidget::UpdateRotation(Vector3& rotation)
{
    ExecuteModifyTransform(rotation, eVector_Rotation);
}

void TransformWidget::UpdateScale(Vector3& scale)
{
    ExecuteModifyTransform(scale, eVector_Scale);
}

void TransformWidget::ExecuteModifyTransform(Vector3 vector, VectorType type)
{
    GameObject* selectedObject = m_window->GetSelectedObject();
    ModifyTransformCommand* command = new ModifyTransformCommand(selectedObject, vector, type);
    CommandManager::Singleton().ExecuteCommand(command);
}