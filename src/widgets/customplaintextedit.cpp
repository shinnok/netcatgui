#include "customplaintextedit.h"

CustomPlainTextEdit::CustomPlainTextEdit(QWidget *parent) :
    QPlainTextEdit(parent)
{
}

void CustomPlainTextEdit::keyPressEvent ( QKeyEvent * e )
{
    if( e->key() == Qt::Key_Enter ||
        e->key() == Qt::Key_Return )
    {
        if (e->modifiers() & Qt::ShiftModifier)
            QPlainTextEdit::keyPressEvent(e);
        else
            emit EnterPressed();
    }else
        QPlainTextEdit::keyPressEvent(e);
}
