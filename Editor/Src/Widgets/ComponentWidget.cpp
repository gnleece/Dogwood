#include "Widgets\ComponentWidget.h"
#include "ComponentModel.h"
#include "ComponentModelItem.h"
#include "ComponentView.h"
#include "DebugLogger.h"
#include "EditorCommands.h"
#include "GameObject.h"
#include "MainEditorWindow.h"
#include "ToolsideGameComponent.h"

#include "..\GeneratedFiles\ui_componentwidget.h"

#include <vector>

using std::vector;

ComponentWidget::ComponentWidget(QWidget* parent, MainEditorWindow* window) : 
  QWidget(parent), m_ui(new Ui::ComponentWidget), m_sourceModel(NULL)
{
    m_ui->setupUi(this);

    m_view = new ComponentView(window);
    m_ui->verticalLayout->addWidget(m_view);

    // Set a reference to this widget in the ModifyTransformCommand class, so that it
    // can tell us when to refresh when transform data gets modified
    EditorCommands::ModifyTransformCommand::sComponentWidget = this;

    connect(m_view, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(OnDoubleClick(const QModelIndex&)));
}

void ComponentWidget::Init(GameObject* go)
{
    // Clear old model
    if (m_sourceModel != NULL)
    {
        m_sourceModel->ClearModel();
        delete m_sourceModel;
    }

    // Refresh the game object's component data, in case the
    // schema has been rebuilt since it was loaded
    vector<ToolsideGameComponent*> components = go->GetToolsideComponentList();
    vector<ToolsideGameComponent*>::iterator iter = components.begin();
    for (; iter != components.end(); iter++)
    {
        (*iter)->ValidateParameters();
    }

    // Prepare new model
    m_sourceModel = new ComponentModel(this, go);
    m_view->setModel(m_sourceModel);
    m_view->setColumnWidth(0, 150);
    m_view->show();
    m_view->expandAll();
}

void ComponentWidget::Refresh()
{
    if (m_sourceModel != NULL)
    {
        m_sourceModel->RefreshModel();
    }
}

void ComponentWidget::OnDoubleClick(const QModelIndex& index)
{
    if (!index.isValid())
        return;

    ComponentModelItem* item = m_sourceModel->GetItem(index);
    if (item != NULL)
    {
        item->OnDoubleClick();
    }
}