#-------------------------------------------------
#
# Project created by QtCreator 2019-03-18T17:40:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

TARGET = HolipDrive
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sharebits.c \
    Crc8.c

HEADERS  += mainwindow.h \
    Holipheader.h \
    api/myapp.h \
    api/myhelper.h \
    sharebits.h \
    Crc8.h

FORMS    += mainwindow.ui
