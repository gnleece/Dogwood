#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include <QtWidgets>

#include "Rendering\Colour.h"
#include "Rendering\Material.h"

class ColorWidget;
class MeshInstance;

namespace Ui
{
    class MeshWidget;
}

class MeshWidget : public QWidget
{
    Q_OBJECT

public:
    MeshWidget(QWidget* parent = 0);

    void                SetMeshInstance(MeshInstance* mesh);

public slots:
    void                UpdateDiffuseColor(ColourRGB& color);
    void                UpdateAmbientColor(ColourRGB& color);
    void                UpdateSpecularColor(ColourRGB& color);

private:
    void                ExecuteColorChange(Material::eMatColourType type, ColourRGB color);

    Ui::MeshWidget*     m_ui;
    MeshInstance*       m_meshInstance;

    ColorWidget*        m_colorWidgets[3];
};