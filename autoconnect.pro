#-------------------------------------------------
#
# Project created by QtCreator 2010-06-04T17:43:04
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = autoconnect
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    autoconnect.cpp

HEADERS += \
    autoconnect.h

CONFIG += mobility
MOBILITY = bearer
