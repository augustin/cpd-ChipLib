#ifndef GRAPHICSSCENELG_H
#define GRAPHICSSCENELG_H

#include <LayerGraphics.h>
#include <QGraphicsScene>
#include "Globals.h"

class GraphicsSceneLG : public LayerGraphics
{
public:
    GraphicsSceneLG(QGraphicsScene* g = 0, bool realLineWidths = false);

    void line(PointList p, qint64 w);
    void poly(PointList p, qint64 w);
    void rect(qint64 x1, qint64 y1, qint64 x2, qint64 y2);

private:
    QGraphicsScene* gs;
    bool rLW;
};

#endif // GRAPHICSSCENELG_H
