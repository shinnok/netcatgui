#include "ncsessionwidget.h"

NcSessionWidget::NcSessionWidget(QWidget *parent, bool EndMessagesWithNewLine)
    : ui(new Ui::NcSessionWidget), end_messages_with_newline(EndMessagesWithNewLine)
{
    ui->setupUi(this);
    ui->sendButton->setEnabled(false);

    QObject::connect(ui->userInputPlainTextEdit, SIGNAL(EnterPressed()), this, SLOT(SendMessage()));
}

NcSessionWidget::~NcSessionWidget()
{
    delete ui;
}

void NcSessionWidget::updateEndMessagesWithNewLine(bool checked)
{
    end_messages_with_newline = checked;
}

bool NcSessionWidget::getEndMessagesWithNewLine()
{
    return end_messages_with_newline;
}

QString NcSessionWidget::getStatusMessage()
{
    return statusMessage;
}

void NcSessionWidget::setStatusMessage(QString message)
{
    statusMessage = message;
}

QString NcSessionWidget::getSessionName()
{
    return sessionName;
}

void NcSessionWidget::setSessionName(QString name)
{
    sessionName = name;
}

QString NcSessionWidget::getSessionLog()
{
    return ui->sessionPlainTextEdit->toPlainText();
}

void NcSessionWidget::selectAll()
{
    if(ui->userInputPlainTextEdit->hasFocus())
       ui->userInputPlainTextEdit->selectAll();
    else
       ui->sessionPlainTextEdit->selectAll();
}

void NcSessionWidget::paste()
{
    ui->userInputPlainTextEdit->paste();
}

void NcSessionWidget::copy()
{
    if(ui->userInputPlainTextEdit->hasFocus())
        ui->userInputPlainTextEdit->copy();
    else
        ui->sessionPlainTextEdit->copy();
}

void NcSessionWidget::Connect()
{
    this->ui->startButton->animateClick();
}

void NcSessionWidget::Disconnect()
{
    this->ui->startButton->animateClick();
}

void NcSessionWidget::SendMessage()
{
    this->ui->sendButton->animateClick();
}

