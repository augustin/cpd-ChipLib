#include "PainterLG.h"

#include <QPainter>

PainterLG::PainterLG(QPainter *painter)
{
    p = painter;
    p->scale(1.0, -1.0);
}

void PainterLG::line(PointList pnts, qint64 w)
{
    QPainterPath path(QPointF(pnts[0]->x, pnts[0]->y));
    for(int i = 1; i < pnts.size(); i++) {
        Point* pnt = pnts.at(i);
        path.lineTo(pnt->x, pnt->y);
    }

    QPen pen;
    pen.setWidth(w);
    //p->setPen(pen);
    p->drawPath(path);
}

void PainterLG::poly(PointList pnts, qint64 w)
{
    QPainterPath path(QPointF(pnts[0]->x, pnts[0]->y));
    for(int i = 1; i < pnts.size(); i++) {
        Point* pnt = pnts.at(i);
        path.lineTo(pnt->x, pnt->y);
    }
    path.lineTo(pnts[0]->x, pnts[0]->y);

    QPen pen;
    pen.setWidth(w);
    //p->setPen(pen);
    p->drawPath(path);
}
