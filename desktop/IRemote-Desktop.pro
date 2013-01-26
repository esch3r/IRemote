#-------------------------------------------------
#
# Project created by QtCreator 2012-12-15T20:28:49
#
#-------------------------------------------------

QT       += core gui network

CONFIG += serialport qwt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IRemote-Desktop
TEMPLATE = app

win32 {
    INCLUDEPATH += ../boost \
                ../qwt6/src
    LIBS += -L../qwt6/lib
}
android {
    INCLUDEPATH += ../boost \
                ../qwt6/src
}
symbian {
    INCLUDEPATH += ../boost \
                ../qwt6/src
}

linux-g++ | linux-g++-64 | linux-g++-32 {
    INCLUDEPATH += /usr/include/qwt5 \  #openSUSE
                /usr/include/qwt6 \  #openSUSE
                /usr/include/qwt \      #Fedora
                /usr/include/qwt-qt4    #ubuntu
}

LIBS += -lqwt


SOURCES += main.cpp\
        iremotewindow.cpp \
    graphicbutton.cpp \
    iremote.cpp \
    showcommanddialog.cpp \
    crc.cpp

HEADERS  += iremotewindow.h \
    graphicbutton.h \
    iremote.h \
    showcommanddialog.h\
    crc.h

FORMS    += iremotewindow.ui \
    showcommanddialog.ui
