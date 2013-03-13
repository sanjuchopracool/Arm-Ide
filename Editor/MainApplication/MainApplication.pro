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
    ToolChainHelpDialog.cpp \
    ProjectData.cpp \
    LinkerConfigDialog.cpp \
    GeneralPlainTextDialog.cpp \
    StartUp.cpp

HEADERS  += mainwindow.h \
    FirstTimeSetupDialog.h \
    ToolChainHelpDialog.h \
    ProjectData.h \
    LinkerConfigDialog.h \
    GeneralPlainTextDialog.h \
    StartUp.h

FORMS += \
    FirstTimeSetupDialog.ui \
    ToolChainHelpDialog.ui \
    LinkerConfigDialog.ui \
    GeneralPlainTextDialog.ui \
    StartUp.ui \
    ProjectSettingDialog.ui

RESOURCES += \
    StartUp.qrc
