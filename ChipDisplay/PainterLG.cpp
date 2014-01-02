#include "PainterLG.h"

#include <QPainter>

PainterLG::PainterLG(QPainter *painter)
{
    p = painter;
    p->scale(1.0, -1.0);
}

void PainterLG::rect(qint64 length, qint64 width, qint64 xpos, qint64 ypos)
{
    p->drawRect(xpos-(length/2), ypos-(width/2), length, width);
}

void PainterLG::line(PointList pnts)
{
    QPainterPath path(QPointF(pnts[0].first, pnts[0].second));
    foreach(Point pnt, pnts) {
        path.lineTo(pnt.first, pnt.second);
    }
    p->drawPath(path);
}

