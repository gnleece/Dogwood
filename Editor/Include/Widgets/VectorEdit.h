#pragma once

// TODO rename this to vectorwidget

#include <QtWidgets>
#include <string>
#include "Math\Algebra.h"

using std::string;

class MainEditorWindow;

namespace Ui
{
    class VectorEdit;
}

class VectorEdit : public QWidget
{
    Q_OBJECT

public:
    VectorEdit(QWidget* parent = 0, MainEditorWindow* window = 0);

    void SetTitle(string title);
    void SetVector(Vector3& vector);

public slots:
    void setX();
    void setY();
    void setZ();

signals:
    void VectorChanged(Vector3& vector);

private:
    float GetFloatFromTextEdit(QPlainTextEdit* textEdit);

    Ui::VectorEdit*     m_ui;
    MainEditorWindow*   m_window;

    Vector3             m_vector;
};
