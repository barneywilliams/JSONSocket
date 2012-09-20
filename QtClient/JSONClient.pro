#-------------------------------------------------
#
# Project created by QtCreator 2012-09-11T22:01:16
#
#-------------------------------------------------

QT       += core network script
QT       -= gui

TARGET = JSONClient
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    client.cpp

HEADERS += \
    client.h

OTHER_FILES += \
    server.rb
