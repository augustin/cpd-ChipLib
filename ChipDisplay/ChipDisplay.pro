#-------------------------------------------------
#
# Project created by QtCreator 2013-11-27T15:43:40
#
#-------------------------------------------------

QT       = core gui opengl svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChipDisplay
TEMPLATE = app

include(../ChipLib/ChipLib.pri)

SOURCES += Main.cpp \
		MainWin.cpp \
    GraphicsSceneLG.cpp \
    PainterLG.cpp

HEADERS  += MainWin.h \
    GraphicsSceneLG.h \
    PainterLG.h

FORMS    += MainWin.ui
