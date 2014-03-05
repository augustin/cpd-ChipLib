#ifndef LAYERGRAPHICS_H
#define LAYERGRAPHICS_H

#include <QList>

class Point {
public:
    Point(qint64 sx, qint64 sy) { x = sx; y = sy; }
    qint64 x, y;
};

typedef QList<Point*> PointList;

class LayerGraphics
{
public:
    virtual void line(PointList p, qint64 w) = 0;
    virtual void poly(PointList p, qint64 w) = 0;
    virtual void rect(qint64 x1, qint64 y1, qint64 x2, qint64 y2) = 0;
};

#endif // LAYERGRAPHICS_H
