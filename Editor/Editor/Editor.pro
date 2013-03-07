TEMPLATE += app
DESTDIR = ../bin
TARGET = Editor
QT += core gui

HEADERS += \
    Editor.h \
    EditorConstants.h \
    CSyntaxHighLighter.h

SOURCES += \
    Editor.cpp \
    main.cpp \
    CSyntaxHighLighter.cpp

OTHER_FILES += \
    CCompleterWords.txt

RESOURCES += \
    Edtior.qrc
