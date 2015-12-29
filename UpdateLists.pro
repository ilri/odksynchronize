#-------------------------------------------------
#
# Project created by QtCreator 2015-07-15T18:43:21
#
#-------------------------------------------------

QT += core gui network xml sql

android:QT += androidextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UpdateLists
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    filedownloader.cpp

android:SOURCES += androidinterface.cpp

HEADERS  += mainwindow.h \
    filedownloader.h

android:HEADERS += androidinterface.h

FORMS    += mainwindow.ui

