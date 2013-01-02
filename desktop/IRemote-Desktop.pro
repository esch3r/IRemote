#-------------------------------------------------
#
# Project created by QtCreator 2012-12-15T20:28:49
#
#-------------------------------------------------

QT       += core gui network

CONFIG += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IRemote-Desktop
TEMPLATE = app


SOURCES += main.cpp\
        iremotewindow.cpp \
    graphicbutton.cpp \
    iremote.cpp

HEADERS  += iremotewindow.h \
    graphicbutton.h \
    iremote.h

FORMS    += iremotewindow.ui
