#-------------------------------------------------
#
# Project created by QtCreator 2016-03-29T09:45:34
#
#-------------------------------------------------

QT       += core gui
QT += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Cs200
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cell.cpp

HEADERS  += mainwindow.h \
    init.h \
    cell.h

FORMS    += mainwindow.ui
RC_FILE  += system.rc

DEFINES  -= UNICODE
QMAKE_CFLAGS +=
QMAKE_CXXFLAGS +=

RESOURCES += \
    resource.qrc

QT+= serialport
QT+=axcontainer

