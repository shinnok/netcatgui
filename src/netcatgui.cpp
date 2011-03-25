#include "netcatgui.h"
#include "util.h"
#include "widgets/ncsessionconnectwidget.h"
#include "widgets/ncsessionlistenwidget.h"

#include "ui_netcatgui.h"

#include <QComboBox>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <QTime>
#include <QIcon>

NetcatGUI::NetcatGUI(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::NetcatGUI)
{
    ui->setupUi(this);
    currentTab = NULL;

    QAction *newListen = new QAction(QIcon(":/toolbar/icons/listen.png"), "Listen", this);
    QAction *newConnect = new QAction(QIcon(":/toolbar/icons/connect.png"),"Connect", this);
    QAction *saveLog = new QAction(QIcon(":/toolbar/icons/save.png"), "Save Log", this);

    newListen->setToolTip("New Listen Tab");
    newConnect->setToolTip("New Connect Tab");

    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->mainToolBar->addAction(newConnect);
    ui->mainToolBar->addAction(newListen);
    ui->mainToolBar->addAction(saveLog);

    QObject::connect(newListen, SIGNAL(triggered()), this, SLOT(ncAddNewListenTab()));
    QObject::connect(newConnect, SIGNAL(triggered()), this, SLOT(ncAddNewConnectTab()));
    QObject::connect(saveLog, SIGNAL(triggered()), this, SLOT(ncSaveLog()));

    QObject::connect(ui->actionSelect_all, SIGNAL(triggered()), this, SLOT(ncSelectAll()));
    QObject::connect(ui->actionPaste, SIGNAL(triggered()), this, SLOT(ncPaste()));
    QObject::connect(ui->actionCopy, SIGNAL(triggered()), this, SLOT(ncCopy()));

    QObject::connect(ui->actionNew_Listen_Tab, SIGNAL(triggered()), this, SLOT(ncAddNewListenTab()));
    QObject::connect(ui->actionNew_Connect_Tab, SIGNAL(triggered()), this, SLOT(ncAddNewConnectTab()));

    QObject::connect(ui->actionClose_All_Tabs, SIGNAL(triggered()), this, SLOT(ncCloseAllTabs()));
    QObject::connect(ui->actionClose_Tab, SIGNAL(triggered()), this, SLOT(ncCloseTab()));

    QObject::connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(ncConnectListen()));
    QObject::connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(ncDisconnect()));

    QObject::connect(ui->actionSend, SIGNAL(triggered()), this, SLOT(ncSend()));

    QObject::connect(ui->actionEnd_Messages_with_new_line, SIGNAL(triggered(bool)), this, SLOT(ncEndMessagesNewLineTriggered(bool)));

    QObject::connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(ncTabCloseRequest(int)));
    QObject::connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(ncCurrentTabChanged(int)));

    QObject::connect(ui->actionSave_Log, SIGNAL(triggered()), this, SLOT(ncSaveLog()));

    QObject::connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(ncQuit()));

    ui->tabWidget->removeTab(0);

    QObject::connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(ncQuit()));

    QObject::connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(ncAbout()));

}

NetcatGUI::~NetcatGUI()
{
    delete ui;
}

void NetcatGUI::ncQuit()
{
    qApp->quit();
}

void NetcatGUI::ncAddNewListenTab()
{
    NcSessionListenWidget *widget = new NcSessionListenWidget(this, ui->actionEnd_Messages_with_new_line->isChecked());
    ui->tabWidget->addTab(widget, QIcon(":/toolbar/icons/listen.png"), "Listen");
}

void NetcatGUI::ncAddNewConnectTab()
{
    NcSessionConnectWidget *widget = new NcSessionConnectWidget(this, ui->actionEnd_Messages_with_new_line->isChecked());
    ui->tabWidget->addTab(widget, QIcon(":/toolbar/icons/connect.png"), "Connect");
}

void NetcatGUI::ncTabCloseRequest(int i)
{
    if(currentTab == ui->tabWidget->widget(i))
        currentTab = NULL;
    ui->tabWidget->widget(i)->close();
    ui->tabWidget->removeTab(i);
}

void NetcatGUI::ncCloseTab()
{
    int index = ui->tabWidget->currentIndex();
    if( index != -1){
          if(currentTab == ui->tabWidget->widget(index))
            currentTab = NULL;
          ui->tabWidget->widget(index)->close();
          ui->tabWidget->removeTab(index);
    }
}

void NetcatGUI::ncCloseAllTabs()
{
    currentTab = NULL;
    for(int i = 0; i < ui->tabWidget->count(); i++){
        ui->tabWidget->widget(i)->close();
    }
    ui->tabWidget->clear();
}

void NetcatGUI::ncConnectListen()
{
    if(ui->tabWidget->currentIndex() == -1){
        QMessageBox::warning(this, "No session active", "You have no session currently active, thus you have nothing to connect.");
        return;
    }
    static_cast<NcSessionWidget*>(ui->tabWidget->currentWidget())->Connect();
}

void NetcatGUI::ncDisconnect()
{
    if(ui->tabWidget->currentIndex() == -1){
        QMessageBox::warning(this, "No session active", "You have no session currently active, thus you have nothing to disconnect.");
        return;
    }
    static_cast<NcSessionWidget*>(ui->tabWidget->currentWidget())->Disconnect();
}

void NetcatGUI::ncSend()
{
    if(ui->tabWidget->currentIndex() == -1){
        QMessageBox::warning(this, "No session active", "You need to have a session active and connected in order to send messages.");
        return;
    }
    static_cast<NcSessionWidget*>(ui->tabWidget->currentWidget())->SendMessage();

}

void NetcatGUI::ncEndMessagesNewLineTriggered(bool checked)
{
    for(int i = 0; i < ui->tabWidget->count(); i++)
        static_cast<NcSessionWidget*>(ui->tabWidget->currentWidget())->updateEndMessagesWithNewLine(checked);

}

void NetcatGUI::ncAbout()
{
    QMessageBox::information(NULL, QString("About ") + NCGUI_PROGRAM_NAME,
                             QString(NCGUI_PROGRAM_NAME) + " v" + NCGUI_VERSION + "\n\n" +
                             "Author: " + NCGUI_AUTHOR + "\n\n" +
                             "Contact E-mail: " + NCGUI_CONTACT_EMAIL + "\n\n" +
                             "License: " + NCGUI_LICENSE);
}

void NetcatGUI::ncCurrentTabChanged(int i)
{
    if(i != -1){
        if(currentTab != NULL)
            QObject::disconnect(currentTab, SIGNAL(statusChanged()), this, SLOT(ncupdateTabStatus()));
        QObject::connect(ui->tabWidget->widget(i), SIGNAL(statusChanged()), this, SLOT(ncupdateTabStatus()));
        currentTab = ui->tabWidget->widget(i);

        ncupdateTabStatus();
    }

}

void NetcatGUI::ncupdateTabStatus()
{
    ui->statusBar->showMessage(static_cast<NcSessionWidget*>(ui->tabWidget->currentWidget())->getStatusMessage());
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), static_cast<NcSessionWidget*>(ui->tabWidget->currentWidget())->getSessionName());
}

void NetcatGUI::ncSaveLog()
{
    if(ui->tabWidget->currentIndex() == -1){
        QMessageBox::warning(this, "No session active", "You have no session currently active, thus you have no log to save.");
        return;
    }
    if(static_cast<NcSessionWidget*>(ui->tabWidget->currentWidget())->getSessionLog().isEmpty()){
        QMessageBox::warning(this, "Session log empty", "The session log is empty, thus you have nothing to save.");
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Session Log"), NULL, tr("Text files (*.txt)"));
    if(fileName.isEmpty())
        return;
    QFile logFile(fileName);
    if(!logFile.open(QIODevice::WriteOnly)){
        QMessageBox::critical(this, "File open error", "The selected file could not be opened for writing.\n Log not saved.");
        return;
    }
    QString log = "--\n[" + QTime::currentTime().toString(Qt::SystemLocaleShortDate) + " " + QDate::currentDate().toString(Qt::SystemLocaleShortDate)
                  + "]" + "\nNetcatGUI log session for " + static_cast<NcSessionWidget*>(ui->tabWidget->currentWidget())->getSessionName() + "\n---\n" + static_cast<NcSessionWidget*>(ui->tabWidget->currentWidget())->getSessionLog().toAscii() + "\n-----";
    qint64 bytesWritten = logFile.write(log.toAscii());
    if( bytesWritten ==  -1)
        QMessageBox::critical(this, "File write error", "Could not write to the specified file.\n Log not saved.");
    else
        if( bytesWritten != qstrlen(log.toAscii()))
        QMessageBox::critical(this, "File write error", "Could not write the entire log session.\n Log saved partially.");
    else
        QMessageBox::information(this, "Log saved successfully", "The session log has been saved successfully.");

    logFile.close();
}

void NetcatGUI::ncSelectAll()
{
    if(ui->tabWidget->currentIndex() == -1){
        QMessageBox::warning(this, "No session active", "You have no session currently active, thus you have nothing to select.");
        return;
    }
    static_cast<NcSessionWidget*>(ui->tabWidget->currentWidget())->selectAll();
}

void NetcatGUI::ncPaste()
{
    if(ui->tabWidget->currentIndex() == -1){
        QMessageBox::warning(this, "No session active", "You have no session currently active, thus you have nowhere to paste.");
        return;
    }
    static_cast<NcSessionWidget*>(ui->tabWidget->currentWidget())->paste();
}

void NetcatGUI::ncCopy()
{
    if(ui->tabWidget->currentIndex() == -1){
        QMessageBox::warning(this, "No session active", "You have no session currently active, thus you have nothing to copy.");
        return;
    }
    static_cast<NcSessionWidget*>(ui->tabWidget->currentWidget())->copy();
}
