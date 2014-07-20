#include "Widgets\VectorEdit.h"

#include "..\GeneratedFiles\ui_vectoredit.h"

VectorEdit::VectorEdit(QWidget* parent)
: m_ui(new Ui::VectorEdit)
{
    m_ui->setupUi(this);
}

void VectorEdit::SetTitle(string title)
{
    m_ui->label_title->setText(QString(title.c_str()));
}
