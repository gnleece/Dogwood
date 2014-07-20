#pragma once

#include <QtWidgets>
#include <string>

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

private:
    Ui::VectorEdit*   m_ui;
};
