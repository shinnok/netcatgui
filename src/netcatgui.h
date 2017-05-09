#ifndef NETCATGUI_H
#define NETCATGUI_H

#include "widgets/ncsessionwidget.h"

#include <QMainWindow>

namespace Ui
{
    class NetcatGUI;
}

class NetcatGUI : public QMainWindow
{
    Q_OBJECT

public:
    NetcatGUI(QWidget *parent = 0);
    ~NetcatGUI();

private:
    Ui::NetcatGUI *ui;
    QWidget *currentTab;

private slots:
    void ncAddNewListenTab();
    void ncAddNewConnectTab();
    void ncTabCloseRequest(int i);
    void ncCloseAllTabs();
    void ncCloseTab();
    void ncConnectListen();
    void ncDisconnect();
    void ncSend();
    void ncEndMessagesNewLineTriggered(bool checked);
    void ncCurrentTabChanged(int i);
    void ncupdateTabStatus();
    void ncSaveLog();
    void ncSelectAll();
    void ncPaste();
    void ncCopy();
    void ncAbout();
    void ncQuit();
};

#endif // NETCATGUI_H
