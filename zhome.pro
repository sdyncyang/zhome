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
        widget.cpp \
    sunraise.cpp \
    myinfo.cpp \
    formsetting.cpp
HEADERS  += widget.h \
    sunraise.h \
    myinfo.h \
    formsetting.h
FORMS    += widget.ui \
    formsetting.ui

target.path = /home/qt_project
INSTALLS += target

RESOURCES += \
    picturelib.qrc
