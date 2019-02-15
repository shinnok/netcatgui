#ifndef CUSTOMPLAINTEXTEDIT_H
#define CUSTOMPLAINTEXTEDIT_H

#include <QPlainTextEdit>

class CustomPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit CustomPlainTextEdit(QWidget *parent = 0);

protected:
    void keyPressEvent ( QKeyEvent * e );

signals:
    void EnterPressed();

public slots:

};

#endif // CUSTOMPLAINTEXTEDIT_H
