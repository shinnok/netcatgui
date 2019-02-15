# -------------------------------------------------
# Project created by QtCreator 2010-03-11T14:45:34
# -------------------------------------------------
QT += core gui widgets network
TARGET = netcatgui
TEMPLATE = app
SOURCES += src/main.cpp \
    src/netcatgui.cpp \
    src/widgets/ncsessionwidget.cpp \
    src/widgets/ncsessionlistenwidget.cpp \
    src/widgets/ncsessionconnectwidget.cpp \
    src/widgets/customplaintextedit.cpp
HEADERS += src/netcatgui.h \
    src/widgets/ncsessionwidget.h \
    src/widgets/ncsessionlistenwidget.h \
    src/widgets/ncsessionconnectwidget.h \
    src/util.h \
    src/widgets/customplaintextedit.h
FORMS += src/netcatgui.ui \
    src/widgets/ncsessionwidget.ui

OTHER_FILES += \
    resources/icons/listen.png \
    resources/icons/icons_readme.txt \
    resources/icons/connect.png \
    resources/ncgui.rc \
    resources/icons/save.png

RESOURCES += \
    resources/resources.qrc

RC_FILE = resources/ncgui.rc
