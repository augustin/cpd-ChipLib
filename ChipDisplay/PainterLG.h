#ifndef PAINTERLG_H
#define PAINTERLG_H

#include <LayerGraphics.h>

class QPainter;

class PainterLG : public LayerGraphics
{
public:
    PainterLG(QPainter* painter = 0, bool realLineWidths = false);

    void line(PointList pnts, qint64 w);
    void poly(PointList pnts, qint64 w);

private:
    QPainter* p;
    bool rLW;
};

#endif // PAINTERLG_H
