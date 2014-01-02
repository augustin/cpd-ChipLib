#ifndef PAINTERLG_H
#define PAINTERLG_H

#include <LayerGraphics.h>

class QPainter;

class PainterLG : public LayerGraphics
{
public:
    PainterLG(QPainter* painter = 0);

    void line(PointList pnts, qint64 w);
    void poly(PointList pnts, qint64 w);

private:
    QPainter* p;
};

#endif // PAINTERLG_H
