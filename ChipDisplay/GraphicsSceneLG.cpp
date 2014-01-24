#include "GraphicsSceneLG.h"

#include <QGraphicsRectItem>
#include "Globals.h"

GraphicsSceneLG::GraphicsSceneLG(QGraphicsScene *g, bool realLineWidths)
{
    gs = g;
    rLW = realLineWidths;
}

void GraphicsSceneLG::line(PointList p, qint64 w)
{
    QPainterPath path(QPointF(p[0]->x, p[0]->y));
    for(int i = 1; i < p.size(); i++) {
        Point* pnt = p.at(i);
        path.lineTo(pnt->x, pnt->y);
    }

    if(rLW) {
        QPen pen;
        pen.setWidth(w);
        gs->addPath(path, pen)->setFlag(QGraphicsItem::ItemIsFocusable);
    } else {
        gs->addPath(path)->setFlag(QGraphicsItem::ItemIsFocusable);
    }
}

void GraphicsSceneLG::poly(PointList p, qint64 w)
{
    QPainterPath path(QPointF(p[0]->x, p[0]->y));
    for(int i = 1; i < p.size(); i++) {
        Point* pnt = p.at(i);
        path.lineTo(pnt->x, pnt->y);
    }
    path.lineTo(p[0]->x, p[0]->y);

    if(rLW) {
        QPen pen;
        pen.setWidth(w);
        gs->addPath(path, pen)->setFlag(QGraphicsItem::ItemIsFocusable);
    } else {
        gs->addPath(path)->setFlag(QGraphicsItem::ItemIsFocusable);
    }
}
