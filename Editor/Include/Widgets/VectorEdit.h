#pragma once

// TODO rename this to vectorwidget

#include <QtWidgets>
#include <string>
#include "Math\Algebra.h"

using std::string;

namespace Ui
{
    class VectorEdit;
}

class VectorEdit : public QWidget
{
    Q_OBJECT

public:
    VectorEdit(QWidget* parent = 0);

    void SetTitle(string title);

public slots:
    void setX();
    void setY();
    void setZ();
    //void setVector(Vector3& vector);

signals:
    void VectorChanged(Vector3& vector);

private:
    float GetFloatFromTextEdit(QPlainTextEdit* textEdit);

    Ui::VectorEdit* m_ui;

    Vector3         m_vector;
};
