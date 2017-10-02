#-------------------------------------------------
#
# Project created by QtCreator 2017-10-02T16:02:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zhome
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui

target.path = /home/qt_project
INSTALLS += target

RESOURCES += \
    picturelib.qrc
