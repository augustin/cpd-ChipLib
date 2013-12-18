#include "GraphicsSceneLG.h"

#include <QGraphicsRectItem>
#include "Globals.h"

GraphicsSceneLG::GraphicsSceneLG(QGraphicsScene *g)
{
    gs = g;
}

void GraphicsSceneLG::rect(qint64 length, qint64 width, qint64 xpos, qint64 ypos, qint64 rotation)
{
    QGraphicsItem* i = gs->addRect(xpos,ypos,length,width,QPen(),QBrush(Qt::black,Qt::SolidPattern));
    i->setRotation(rotation);
    i->setFlag(QGraphicsItem::ItemIsFocusable);
}
