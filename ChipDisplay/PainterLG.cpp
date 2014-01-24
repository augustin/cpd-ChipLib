#include "PainterLG.h"

#include <QPainter>

PainterLG::PainterLG(QPainter *painter, bool realLineWidths)
{
    p = painter;
    p->scale(1.0, -1.0);
    rLW = realLineWidths;
}

void PainterLG::line(PointList pnts, qint64 w)
{
    QPainterPath path(QPointF(pnts[0]->x, pnts[0]->y));
    for(int i = 1; i < pnts.size(); i++) {
        Point* pnt = pnts.at(i);
        path.lineTo(pnt->x, pnt->y);
    }

    if(rLW) {
        QPen pen;
        pen.setWidth(w);
        p->setPen(pen);
        p->drawPath(path);
    } else {
        p->drawPath(path);
    }
}

void PainterLG::poly(PointList pnts, qint64 w)
{
    QPainterPath path(QPointF(pnts[0]->x, pnts[0]->y));
    for(int i = 1; i < pnts.size(); i++) {
        Point* pnt = pnts.at(i);
        path.lineTo(pnt->x, pnt->y);
    }
    path.lineTo(pnts[0]->x, pnts[0]->y);

    if(rLW) {
        QPen pen;
        pen.setWidth(w);
        p->setPen(pen);
        p->drawPath(path);
    } else {
        p->drawPath(path);
    }
}
