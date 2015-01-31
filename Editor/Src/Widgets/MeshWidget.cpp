#include "Widgets\MeshWidget.h"

#include "EditorCommands.h"
#include "Rendering\Material.h"
#include "Rendering\MeshInstance.h"
#include "Widgets\ColorWidget.h"
#include "..\GeneratedFiles\ui_meshwidget.h"
#include "MainEditorWindow.h"

#include <string>

using namespace EditorCommands;

MeshWidget::MeshWidget(QWidget* parent)
    : QWidget(parent), m_ui(new Ui::MeshWidget), m_meshInstance(NULL)
{
    m_ui->setupUi(this);
    setFixedHeight(240);

    // Create color widgets
    for (int i = 0; i < 3; i++)
    {
        m_colorWidgets[i] = new ColorWidget(this);
    }
    m_ui->colorDiffuseLayout->addWidget(m_colorWidgets[0]);
    m_ui->colorAmbientLayout->addWidget(m_colorWidgets[1]);
    m_ui->colorSpecularLayout->addWidget(m_colorWidgets[2]);

    // Connect to color changed signals
    connect(m_colorWidgets[0], SIGNAL(ColorChanged(ColourRGB&)), this, SLOT(UpdateDiffuseColor(ColourRGB&)));
    connect(m_colorWidgets[1], SIGNAL(ColorChanged(ColourRGB&)), this, SLOT(UpdateAmbientColor(ColourRGB&)));
    connect(m_colorWidgets[2], SIGNAL(ColorChanged(ColourRGB&)), this, SLOT(UpdateSpecularColor(ColourRGB&)));
}

void MeshWidget::SetMeshInstance(MeshInstance* mesh)
{
    m_meshInstance = mesh;

    if (m_meshInstance)
    {
        m_colorWidgets[0]->SetColor(m_meshInstance->GetMaterial()->GetColour(Material::MAT_COLOUR_DIFFUSE));
        m_colorWidgets[1]->SetColor(m_meshInstance->GetMaterial()->GetColour(Material::MAT_COLOUR_AMBIENT));
        m_colorWidgets[2]->SetColor(m_meshInstance->GetMaterial()->GetColour(Material::MAT_COLOUR_SPECULAR));
    }
}

void MeshWidget::UpdateDiffuseColor(ColourRGB& color)
{
    ExecuteColorChange(Material::MAT_COLOUR_DIFFUSE, color);
}

void MeshWidget::UpdateAmbientColor(ColourRGB& color)
{
    ExecuteColorChange(Material::MAT_COLOUR_AMBIENT, color);
}

void MeshWidget::UpdateSpecularColor(ColourRGB& color)
{
    ExecuteColorChange(Material::MAT_COLOUR_SPECULAR, color);
}

void MeshWidget::ExecuteColorChange(Material::eMatColourType type, ColourRGB color)
{
    if (m_meshInstance)
    {
        Material* mat = m_meshInstance->GetMaterial();
        ChangeMaterialColorCommand* command = new ChangeMaterialColorCommand(mat, (MaterialColorType)type, color);
        CommandManager::Singleton().ExecuteCommand(command);
    }
}