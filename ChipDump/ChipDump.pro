#-------------------------------------------------
#
# Project created by QtCreator 2014-01-24T15:16:18
#
#-------------------------------------------------

QT  = core gui

TARGET = ChipDump
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

include(../ChipLib/ChipLib.pri)

SOURCES += Main.cpp \
            ../ChipDisplay/PainterLG.cpp

HEADERS += ../ChipDisplay/PainterLG.h
