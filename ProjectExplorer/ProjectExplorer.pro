#-------------------------------------------------
#
# Project created by QtCreator 2013-03-19T22:47:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProjectExplorer
TEMPLATE = app


SOURCES += main.cpp \
    ProjectExplorer.cpp \
    ProjectModel.cpp \
    ../Editor/MainApplication/ProjectData.cpp

HEADERS  += \
    ProjectExplorer.h \
    ProjectModel.h \
    ../Editor/MainApplication/ProjectData.h

OTHER_FILES += \
    as.txt

RESOURCES += \
    ProjectExplorer.qrc
