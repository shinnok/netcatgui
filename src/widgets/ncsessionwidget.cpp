#include "ncsessionwidget.h"

NcSessionWidget::NcSessionWidget(QWidget *parent, bool EndMessagesWithNewLine, Encoding encoding)
    : QWidget(parent), ui(new Ui::NcSessionWidget), end_messages_with_newline(EndMessagesWithNewLine), text_encoding(encoding)
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

void NcSessionWidget::updateTextEncoding(Encoding encoding)
{
    text_encoding = encoding;
}

bool NcSessionWidget::getEndMessagesWithNewLine()
{
    return end_messages_with_newline;
}

NcSessionWidget::Encoding NcSessionWidget::getTextEncoding()
{
    return text_encoding;
}

QString NcSessionWidget::getTextEncodingName()
{
    switch (getTextEncoding())
    {
    case Latin1:
        return tr("Latin-1");
    case Utf8:
        return tr("UTF-8");
    case System:
        return tr("System");
    }
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

