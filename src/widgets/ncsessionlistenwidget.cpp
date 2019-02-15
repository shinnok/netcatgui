#include "ncsessionlistenwidget.h"
#include <QHostInfo>
#include <QMessageBox>
#include <QLineEdit>

NcSessionListenWidget::NcSessionListenWidget(QWidget *parent, bool EndMessagesWithNewLine, Encoding encoding):
                                             NcSessionWidget(parent, EndMessagesWithNewLine, encoding), hostConnection(NULL)
{
    setSessionName("Listen");
    setStatusMessage("Disconnected");
    ui->headerGroupBox->setTitle("Listen on");
    ui->hostnameLabel->setText("Interface:");
    ui->portLabel->setText("Port:");

    ui->startButton->setText("Listen");
    ui->addressComboBox->setToolTip("Interface to listen on");
    ui->portComboBox->setToolTip("Port to listen on");

    ui->addressComboBox->setEditable(false);
    /*add local interfaces to the address combobox*/
    QHostInfo localInterfaces = QHostInfo::fromName(QHostInfo::localHostName());
    for(int i = 0; i < localInterfaces.addresses().size(); i++)
            ui->addressComboBox->addItem(localInterfaces.addresses().at(i).toString());
    ui->addressComboBox->addItem(QHostAddress(QHostAddress::LocalHost).toString());
    ui->addressComboBox->addItem(QHostAddress(QHostAddress::LocalHostIPv6).toString());
    ui->addressComboBox->addItem(QHostAddress(QHostAddress::Any).toString());
    ui->addressComboBox->setCurrentIndex(ui->addressComboBox->count()-1);
    ui->addressComboBox->addItem(QHostAddress(QHostAddress::AnyIPv6).toString());
    ui->addressComboBox->addItem(QHostAddress(QHostAddress::Broadcast).toString());


    QObject::connect(ui->startButton, SIGNAL(clicked()), this, SLOT(startListen()));
    QObject::connect(ui->sendButton, SIGNAL(clicked()), this, SLOT(sendMessageToClient()));
    /*tcp listen server setup*/
    connect(&tcpListenServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
}

NcSessionListenWidget::~NcSessionListenWidget()
{
    tcpListenServer.close();
}

void NcSessionListenWidget::startListen()
{
    QString port;

    if(tcpListenServer.isListening()){
        QMessageBox::warning(this, "Already Listening", "You already started listening.");
        return;
    }

    port = ui->portComboBox->currentText();

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

    if(!tcpListenServer.listen(QHostAddress(ui->addressComboBox->currentText()), port.toInt())){
        QMessageBox::critical(this, "Cannot start listen",
                              "Could not bind to the specified port and interface.\n"
                                                                + tcpListenServer.errorString());
        return;
    }else
        bindSuccess();

}

void NcSessionListenWidget::stopListen()
{
    if(!tcpListenServer.isListening()){
        QMessageBox::warning(this, "Not Listening", "Listening not started.");
        return;
    }

    tcpListenServer.close();

    setStatusMessage("Disconnected");
    emit statusChanged();

    ui->startButton->setText(tr("Listen"));
    QObject::disconnect(ui->startButton, SIGNAL(clicked()), this, SLOT(stopListen()));
    QObject::connect(ui->startButton, SIGNAL(clicked()), this, SLOT(startListen()));

}

void NcSessionListenWidget::bindSuccess()
{
    setSessionName(tcpListenServer.serverAddress().toString() + ":" + QString::number(tcpListenServer.serverPort()));
    setStatusMessage("Listening");
    emit statusChanged();

    ui->startButton->setText(tr("Stop"));
    QObject::disconnect(ui->startButton, SIGNAL(clicked()), this, SLOT(startListen()));
    QObject::connect(ui->startButton, SIGNAL(clicked()), this, SLOT(stopListen()));
}

void NcSessionListenWidget::hostDisconnect()
{
    if(hostConnection == NULL || !hostConnection->isOpen()){
        QMessageBox::warning(this, "Not Connected", "You need to be connected in order to disconnect.");
        return;
    }

    hostConnection->close();
    delete hostConnection;
    hostConnection = NULL;
    setStatusMessage("Listening");
    emit statusChanged();
    ui->startButton->setText(tr("Stop"));
    QObject::disconnect(ui->startButton, SIGNAL(clicked()), this, SLOT(hostDisconnect()));
    QObject::connect(ui->startButton, SIGNAL(clicked()), this, SLOT(stopListen()));
    ui->sendButton->setEnabled(false);
}

void NcSessionListenWidget::hostConnectionError(QAbstractSocket::SocketError socketError)
{
    hostDisconnect();

    if (socketError == QTcpSocket::RemoteHostClosedError){
        QMessageBox::critical(this, tr("Connection closed"), tr("The remote host closed the connection."));
        return;
    }

    QMessageBox::critical(this, tr("Network error"),
                                  tr("The following network error occurred: %1.")
                                  .arg(hostConnection->errorString()));
}

void NcSessionListenWidget::acceptConnection()
{
    hostConnection = tcpListenServer.nextPendingConnection();
    QObject::connect(hostConnection, SIGNAL(readyRead()), this, SLOT(connectionDataAvailable()));
    QObject::connect(hostConnection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(hostConnectionError(QAbstractSocket::SocketError)));

    setStatusMessage("Connected");
    emit statusChanged();

    ui->startButton->setText(tr("Disconnect"));
    QObject::disconnect(ui->startButton, SIGNAL(clicked()), this, SLOT(stopListen()));
    QObject::connect(ui->startButton, SIGNAL(clicked()), this, SLOT(hostDisconnect()));
    ui->sendButton->setEnabled(true);
}

void NcSessionListenWidget::sendMessageToClient()
{

    if(hostConnection == NULL || !hostConnection->isOpen()){
        QMessageBox::warning(this, "Cannot send message", "You must have a client connected before trying to send messages.");
        return;
    }

    QString message = ui->userInputPlainTextEdit->toPlainText();
    if(!message.isEmpty()){
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
        hostConnection->write(messageData);
    }
    ui->userInputPlainTextEdit->clear();
}

void NcSessionListenWidget::connectionDataAvailable()
{
    ui->sessionPlainTextEdit->appendPlainText(QString(hostConnection->readAll()));
}
