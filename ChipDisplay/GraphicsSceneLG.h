#ifndef GRAPHICSSCENELG_H
#define GRAPHICSSCENELG_H

#include <LayerGraphics.h>
#include <QGraphicsScene>
#include "Globals.h"

class GraphicsSceneLG : public LayerGraphics
{
public:
    GraphicsSceneLG(QGraphicsScene* g = 0);
    void rect(qint64 length, qint64 width, qint64 xpos, qint64 ypos, qint64 rotation = 0);
    void line(PointList p);

private:
    QGraphicsScene* gs;
};

#endif // GRAPHICSSCENELG_H
