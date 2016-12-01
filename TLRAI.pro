#-------------------------------------------------
#
# Project created by QtCreator 2016-11-30T22:29:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TLRAI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    convnets.cpp \
    imagereader.cpp

HEADERS  += mainwindow.h \
    convnets.h \
    imagereader.h

FORMS    += mainwindow.ui
