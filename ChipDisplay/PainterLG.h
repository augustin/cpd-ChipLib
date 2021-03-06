#ifndef PAINTERLG_H
#define PAINTERLG_H

#include <LayerGraphics.h>
#include <QRect>

#define SCALE_FACTOR 30

class QPainter;

class PainterLG : public LayerGraphics
{
public:
	PainterLG(QPainter* painter = 0, bool realLineWidths = false, QRect bR = QRect());

    void line(PointList pnts, qint64 w);
    void poly(PointList pnts, qint64 w);
    void rect(qint64 x1, qint64 y1, qint64 x2, qint64 y2);

private:
    QPainter* p;
    bool rLW;
};

#endif // PAINTERLG_H
