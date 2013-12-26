#include "GraphicsSceneLG.h"

#include <QGraphicsRectItem>
#include "Globals.h"

GraphicsSceneLG::GraphicsSceneLG(QGraphicsScene *g)
{
    gs = g;
}

void GraphicsSceneLG::rect(qint64 length, qint64 width, qint64 xpos, qint64 ypos)
{
    QGraphicsItem* i = gs->addRect(xpos-(length/2), ypos-(width/2), length, width);
    i->setFlag(QGraphicsItem::ItemIsFocusable);
}

void GraphicsSceneLG::line(PointList p)
{
    QPainterPath path(QPointF(p[0].first, p[0].second));
    foreach(Point pnt, p) {
        path.lineTo(pnt.first, pnt.second);
    }
    gs->addPath(path);
}
