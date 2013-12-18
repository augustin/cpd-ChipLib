#ifndef LAYERGRAPHICS_H
#define LAYERGRAPHICS_H

#include <qglobal.h>

class LayerGraphics
{
public:
    virtual void rect(qint64 length, qint64 width, qint64 xpos, qint64 ypos, qint64 rotation = 0) = 0;
};

#endif // LAYERGRAPHICS_H
