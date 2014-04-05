#-------------------------------------------------
#
# Project created by QtCreator 2014-04-03T22:09:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qfm
TEMPLATE = app


SOURCES += main.cpp\
        dirwindow.cpp \
    iconview.cpp \
    filesystemmodel.cpp

HEADERS  += dirwindow.h \
    iconview.h \
    filesystemmodel.h \
    qfmdeclarations.h

FORMS    += dirwindow.ui
