#ifndef NCSESSIONLISTENWIDGET_H
#define NCSESSIONLISTENWIDGET_H

#include "ncsessionwidget.h"
#include <QTcpSocket>
#include <QTcpServer>

class NcSessionListenWidget : public NcSessionWidget
{

    Q_OBJECT

public:
    NcSessionListenWidget(QWidget *parent = 0, bool EndMessagesWithNewLine = false, Encoding encoding = Encoding::Utf8);
    ~NcSessionListenWidget();

private slots:
    void startListen();
    void stopListen();
    void bindSuccess();
    void hostDisconnect();
    void hostConnectionError(QAbstractSocket::SocketError socketError);
    void acceptConnection();
    void connectionDataAvailable();
    void sendMessageToClient();

private:
    QTcpServer tcpListenServer;
    QTcpSocket *hostConnection;

};

#endif // NCSESSIONLISTENWIDGET_H
