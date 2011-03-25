#ifndef NC_SESSION_WIDGET_H
#define NC_SESSION_WIDGET_H

#include "ui_ncsessionwidget.h"
#include <QWidget>

namespace Ui
{
    class NcSessionWidget;
}

class NcSessionWidget : public QWidget
{
    Q_OBJECT

public:
    NcSessionWidget(QWidget *parent = 0, bool EndMessagesWithNewLine = false);
    ~NcSessionWidget();
    QString getStatusMessage();
    QString getSessionName();
    QString getSessionLog();
    void selectAll();
    void paste();
    void copy();
    void updateEndMessagesWithNewLine(bool checked);
    bool getEndMessagesWithNewLine();

public slots:
    virtual void Connect();
    virtual void Disconnect();
    virtual void SendMessage();

protected:
    void setSessionName(QString host);
    void setStatusMessage(QString message);

protected:
    Ui::NcSessionWidget *ui;

signals:
    void statusChanged();

private:
    QString sessionName;
    QString statusMessage;

    bool    end_messages_with_newline;
};

#endif // NC_SESSION_WIDGET_H
