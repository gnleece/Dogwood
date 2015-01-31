#include "Widgets\VectorWidget.h"

#include "..\GeneratedFiles\ui_VectorWidget.h"

VectorWidget::VectorWidget(QWidget* parent)
: QWidget(parent), m_ui(new Ui::VectorWidget)
{
    m_ui->setupUi(this);

    // only allow numerical input   // TODO this doesn't seem to work?
    m_ui->textEdit_x->setInputMethodHints(Qt::ImhDigitsOnly);
    m_ui->textEdit_y->setInputMethodHints(Qt::ImhDigitsOnly);
    m_ui->textEdit_z->setInputMethodHints(Qt::ImhDigitsOnly);

    // disable built-in undo/redo, since the engine will handle this
    m_ui->textEdit_x->setUndoRedoEnabled(false);
    m_ui->textEdit_y->setUndoRedoEnabled(false);
    m_ui->textEdit_z->setUndoRedoEnabled(false);

    // connect to text edit signals (so that we know when the values get changed)
    connect(m_ui->textEdit_x, SIGNAL(textChanged()), this, SLOT(setX()));
    connect(m_ui->textEdit_y, SIGNAL(textChanged()), this, SLOT(setY()));
    connect(m_ui->textEdit_z, SIGNAL(textChanged()), this, SLOT(setZ()));
}

void VectorWidget::SetTitle(string title)
{
    m_ui->label_title->setText(QString(title.c_str()));
}

void VectorWidget::setX()
{
    m_vector[0] = GetFloatFromTextEdit(m_ui->textEdit_x);
    emit VectorChanged(m_vector);
}

void VectorWidget::setY()
{
    m_vector[1] = GetFloatFromTextEdit(m_ui->textEdit_y);
    emit VectorChanged(m_vector);
}

void VectorWidget::setZ()
{
    m_vector[2] = GetFloatFromTextEdit(m_ui->textEdit_z);
    emit VectorChanged(m_vector);
}

void VectorWidget::SetVector(Vector3& vector)
{
    m_vector = vector;

    // block "vector changed" signals, because we are just displaying new values, not changing them
    this->blockSignals(true);
    m_ui->textEdit_x->setPlainText(QString::number(m_vector[0]));
    m_ui->textEdit_y->setPlainText(QString::number(m_vector[1]));
    m_ui->textEdit_z->setPlainText(QString::number(m_vector[2]));
    this->blockSignals(false);
}

float VectorWidget::GetFloatFromTextEdit(QPlainTextEdit* textEdit)
{
    // TODO error checking
    QString str = textEdit->toPlainText();
    float newValue = atof(str.toStdString().c_str());
    return newValue;
}
