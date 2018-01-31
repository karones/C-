#-------------------------------------------------
#
# Project created by QtCreator 2017-11-06T13:16:23
#
#-------------------------------------------------

QT       += core
QT       += core network
QT       -= gui

TARGET = cmm_kays
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    tcpserver.cpp \
    configurator.cpp \
    kays.cpp \
    csv_parcer.cpp

HEADERS += \
    tcpserver.h \
    configurator.h \
    main.h \
    kays.h \
    csv_parcer.h
QMAKE_CXXFLAGS += -std=c++11 -pthread -lpthread

DISTFILES += \
    config.ini
