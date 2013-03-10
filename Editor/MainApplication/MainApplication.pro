#-------------------------------------------------
#
# Project created by QtCreator 2013-03-10T12:40:15
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MainApplication
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    FirstTimeSetupDialog.cpp \
    ToolChainHelpDialog.cpp

HEADERS  += mainwindow.h \
    FirstTimeSetupDialog.h \
    ToolChainHelpDialog.h

FORMS += \
    FirstTimeSetupDialog.ui \
    ToolChainHelpDialog.ui
