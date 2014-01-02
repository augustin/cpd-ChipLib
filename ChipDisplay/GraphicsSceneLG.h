#ifndef GRAPHICSSCENELG_H
#define GRAPHICSSCENELG_H

#include <LayerGraphics.h>
#include <QGraphicsScene>
#include "Globals.h"

class GraphicsSceneLG : public LayerGraphics
{
public:
    GraphicsSceneLG(QGraphicsScene* g = 0);

    void line(PointList p, qint64 w);
    void poly(PointList p, qint64 w);

private:
    QGraphicsScene* gs;
};

#endif // GRAPHICSSCENELG_H
