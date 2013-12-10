#-------------------------------------------------
#
# Project created by QtCreator 2013-11-27T15:43:40
#
#-------------------------------------------------

QT       = core gui opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CIFdisplay
TEMPLATE = app

include(../ChipLib/ChipLib.pri)

SOURCES += Main.cpp \
		MainWin.cpp \
	GraphicsLayerView.cpp

HEADERS  += MainWin.h \
	GraphicsLayerView.h

FORMS    += MainWin.ui
