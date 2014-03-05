#include "PainterLG.h"

#include <QPainter>

PainterLG::PainterLG(QPainter *painter, bool realLineWidths, QRect bR)
{
    p = painter;

    p->scale(1.0/SCALE_FACTOR, -1.0/SCALE_FACTOR);
    QTransform move;
    move.translate(-bR.left(), -bR.top()+1);
    p->setTransform(move, true);
    p->setBrush(QBrush(QColor(0, 0, 0, 100).toRgb()));

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

void PainterLG::rect(qint64 x1, qint64 y1, qint64 x2, qint64 y2)
{
    p->drawRect(x1, y1, x2-x1, y1-y2);
}
