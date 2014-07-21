#include "Widgets\VectorEdit.h"

#include "..\GeneratedFiles\ui_vectoredit.h"

VectorEdit::VectorEdit(QWidget* parent)
: m_ui(new Ui::VectorEdit)
{
    m_ui->setupUi(this);

    // only allow numerical input   // TODO this doesn't seem to work?
    m_ui->textEdit_x->setInputMethodHints(Qt::ImhDigitsOnly);
    m_ui->textEdit_y->setInputMethodHints(Qt::ImhDigitsOnly);
    m_ui->textEdit_z->setInputMethodHints(Qt::ImhDigitsOnly);

    // connect to text edit signals (so that we know when the values get changed)
    connect(m_ui->textEdit_x, SIGNAL(textChanged()), this, SLOT(setX()));
    connect(m_ui->textEdit_y, SIGNAL(textChanged()), this, SLOT(setY()));
    connect(m_ui->textEdit_z, SIGNAL(textChanged()), this, SLOT(setZ()));
}

void VectorEdit::SetTitle(string title)
{
    m_ui->label_title->setText(QString(title.c_str()));
}

void VectorEdit::setX()
{
    m_vector[0] = GetFloatFromTextEdit(m_ui->textEdit_x);
    emit VectorChanged(m_vector);
}

void VectorEdit::setY()
{
    m_vector[1] = GetFloatFromTextEdit(m_ui->textEdit_y);
    emit VectorChanged(m_vector);
}

void VectorEdit::setZ()
{
    m_vector[2] = GetFloatFromTextEdit(m_ui->textEdit_z);
    emit VectorChanged(m_vector);
}

float VectorEdit::GetFloatFromTextEdit(QPlainTextEdit* textEdit)
{
    // TODO error checking
    QString str = textEdit->toPlainText();
    float newValue = atof(str.toStdString().c_str());
    return newValue;
}
