#ifndef LAYERGRAPHICS_H
#define LAYERGRAPHICS_H

#include <QtGlobal>
#include <QList>
#include <QPair>

typedef QPair<qint64, qint64> Point;
typedef QList<Point> PointList;

class LayerGraphics
{
public:
    virtual void rect(qint64 length, qint64 width, qint64 xpos, qint64 ypos) = 0;
    virtual void line(PointList p) = 0;
};

#endif // LAYERGRAPHICS_H
