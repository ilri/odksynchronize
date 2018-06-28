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
    filedownloader.cpp \
    prjlistmodel.cpp

android:SOURCES += androidinterface.cpp

HEADERS  += mainwindow.h \
    filedownloader.h \
    prjlistmodel.h

android:HEADERS += androidinterface.h

FORMS    += mainwindow.ui

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

