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
    LinkerConfigDialog.cpp \
    GeneralPlainTextDialog.cpp \
    StartUp.cpp \
    NewProject.cpp \
    ProjectData.cpp \
    ProjectSettingDialog.cpp \
    SoftwareDefaults.cpp \
    ProjectExplorer.cpp \
    ProjectModel.cpp

HEADERS  += mainwindow.h \
    FirstTimeSetupDialog.h \
    ToolChainHelpDialog.h \
    ProjectData.h \
    LinkerConfigDialog.h \
    GeneralPlainTextDialog.h \
    StartUp.h \
    NewProject.h \
    ProjectSettingDialog.h \
    SoftwareDefaults.h \
    ProjectModel.h \
    ProjectExplorer.h

FORMS += \
    FirstTimeSetupDialog.ui \
    ToolChainHelpDialog.ui \
    LinkerConfigDialog.ui \
    GeneralPlainTextDialog.ui \
    StartUp.ui \
    ProjectSettingDialog.ui \
    NewProject.ui

RESOURCES += \
    StartUp.qrc \
    ProjectExplorer.qrc
