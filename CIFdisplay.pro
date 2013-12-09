#-------------------------------------------------
#
# Project created by QtCreator 2013-11-27T15:43:40
#
#-------------------------------------------------

QT       = core gui opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CIFdisplay
TEMPLATE = app


SOURCES += Main.cpp \
        MainWin.cpp \
    Globals.cpp \
    Cif.cpp \
    GraphicsLayerView.cpp

HEADERS  += MainWin.h \
    Globals.h \
    Cif.h \
    GraphicsLayer.h \
    GraphicsLayerView.h

FORMS    += MainWin.ui
