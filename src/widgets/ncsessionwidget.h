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
    enum Encoding
    {
        Latin1,
        Utf8,
        System
    };

    NcSessionWidget(QWidget *parent = 0, bool EndMessagesWithNewLine = false, Encoding encoding = Utf8);
    ~NcSessionWidget();
    QString getStatusMessage();
    QString getSessionName();
    QString getSessionLog();
    void selectAll();
    void paste();
    void copy();
    void updateEndMessagesWithNewLine(bool checked);
    void updateTextEncoding(Encoding encoding);
    bool getEndMessagesWithNewLine();
    Encoding getTextEncoding();
    QString  getTextEncodingName();

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
    Encoding text_encoding;
};

#endif // NC_SESSION_WIDGET_H
