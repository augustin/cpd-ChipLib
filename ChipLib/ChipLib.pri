# QMakeFile for ChipLib -- The Chip Reader Library

SOURCES +=  $$PWD/Chip.cpp \
  $$PWD/Globals.cpp \
  $$PWD/IO/Cif.cpp

HEADERS +=  $$PWD/Chip.h \
  $$PWD/Globals.h \
  $$PWD/LayerGraphics.h \
  $$PWD/IO/Cif.h

HEADERS += $$PWD/IO/Cif.h \
  $$PWD/Globals.h \
  $$PWD/LayerGraphics.h \
  $$PWD/Chip.h

INCLUDEPATH += $$PWD
