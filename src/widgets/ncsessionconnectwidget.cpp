#include "ncsessionconnectwidget.h"
#include <QMessageBox>
#include <QHostInfo>
#include <QHostAddress>
#include <QLineEdit>

NcSessionConnectWidget::NcSessionConnectWidget(QWidget *parent, bool EndMessagesWithNewLine, NcSessionWidget::Encoding encoding):
                                               NcSessionWidget(parent, EndMessagesWithNewLine, encoding)
{
    setSessionName("Connect");
    setStatusMessage("Disconnected");
    ui->headerGroupBox->setTitle("Connect to");
    ui->hostnameLabel->setText("Hostname:");
    ui->portLabel->setText("Port:");

    ui->startButton->setText("Connect");
    ui->addressComboBox->setToolTip("Hostname to connect to");
    ui->portComboBox->setToolTip("Port to connect to");

    QObject::connect(ui->startButton, SIGNAL(clicked()), this, SLOT(startConnect()));
    QObject::connect(ui->sendButton, SIGNAL(clicked()), this, SLOT(sendMessageToHost()));
    /*host connection setup*/
    QObject::connect(&hostConnection, SIGNAL(connected()), this, SLOT(connectSuccess()));
    QObject::connect(&hostConnection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectFailure(QAbstractSocket::SocketError)));
    QObject::connect(&hostConnection, SIGNAL(readyRead()), this, SLOT(connectionDataAvailable()));
}

NcSessionConnectWidget::~NcSessionConnectWidget()
{
    hostConnection.close();
}

void NcSessionConnectWidget::startConnect()
{
    QString hostname, port;

    if(hostConnection.isOpen()){
        QMessageBox::warning(this, "Already Connected", "You are already connected. Disconnect first.");
        return;
    }

    hostname = ui->addressComboBox->currentText();
    port = ui->portComboBox->currentText();

    if(hostname.isEmpty()){
        QMessageBox::warning(this, "No host specified", "You need to type a hostname in either domain or ip address to connect to.");
        return;
    }
    if(ui->addressComboBox->findText(hostname) == -1)
        ui->addressComboBox->insertItem(0, hostname);
    if(port.isEmpty()){
        QMessageBox::warning(this, "No port specified", "You need to specify port number to connect to.");
        return;
    }
    if(port.toInt() > 65535 || port.toInt() < 1){
        QMessageBox::warning(this, "Invalid port", "The port number you typed is invalid. Valid range is 1-65535.");
        return;
    }
    if(ui->portComboBox->findText(port) == -1)
        ui->portComboBox->insertItem(0, port);

    QHostInfo hostInfo = QHostInfo::fromName(hostname);
    if(hostInfo.error() != QHostInfo::NoError){
        QMessageBox::critical(this, "Hostname fail", "The hostname dns lookup has failed or ip address invalid.");
        return;
    }
    /*initiate connection*/
    hostConnection.connectToHost(hostInfo.hostName(), port.toInt());

    ui->startButton->setText(tr("Connecting..."));
    setStatusMessage("Connecting...");
    emit statusChanged();
    QObject::disconnect(ui->startButton, SIGNAL(clicked()), this, SLOT(startConnect()));
    QObject::connect(ui->startButton, SIGNAL(clicked()), this, SLOT(disconnect()));
}

void NcSessionConnectWidget::disconnect()
{
    if(!hostConnection.isOpen()){
        QMessageBox::warning(this, "Not Connected", "You need to be connected in order to disconnect.");
        return;
    }

    hostConnection.abort();
    hostConnection.close();
    setStatusMessage("Disconnected");
    emit statusChanged();
    ui->startButton->setText(tr("Connect"));
    QObject::disconnect(ui->startButton, SIGNAL(clicked()), this, SLOT(disconnect()));
    QObject::connect(ui->startButton, SIGNAL(clicked()), this, SLOT(startConnect()));
    ui->sendButton->setEnabled(false);
}

void NcSessionConnectWidget::connectSuccess()
{
    setSessionName(hostConnection.peerName() + ":" + QString::number(hostConnection.peerPort()));
    setStatusMessage("Connected");
    emit statusChanged();
    ui->startButton->setText(tr("Disconnect"));
    ui->sendButton->setEnabled(true);
}

void NcSessionConnectWidget::connectFailure(QAbstractSocket::SocketError socketError)
{
    disconnect();
    if(socketError == QTcpSocket::ConnectionRefusedError){
        QMessageBox::critical(this, tr("Connection refused"), tr("The connection was refused by the peer (or timed out)."));
        return;
    }

    if (socketError == QTcpSocket::RemoteHostClosedError){
        QMessageBox::critical(this, tr("Connection closed"), tr("The remote host closed the connection."));
        return;
    }

    QMessageBox::critical(this, tr("Network error"),
                                  tr("The following network error occurred: %1.")
                                  .arg(hostConnection.errorString()));
}

void NcSessionConnectWidget::connectionDataAvailable()
{
    ui->sessionPlainTextEdit->appendPlainText(QString(hostConnection.readAll()));
}

void NcSessionConnectWidget::sendMessageToHost()
{
    QString message = ui->userInputPlainTextEdit->toPlainText();

    if(!hostConnection.isOpen()){
        QMessageBox::warning(this, "Cannot send message", "Connect first before trying to send messages.");
        return;
    }

    if(!message.isEmpty()) {
        if(getEndMessagesWithNewLine())
            message.append("\n");
        QByteArray messageData;
        switch (getTextEncoding()) {
        case Latin1:
            messageData = message.toLatin1();
            break;
        case Utf8:
            messageData = message.toUtf8();
            break;
        case System:
            messageData = message.toLocal8Bit();
            break;
        }
        hostConnection.write(messageData);
    }
    ui->userInputPlainTextEdit->clear();
}

