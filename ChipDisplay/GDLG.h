#ifndef GDLG_H
#define GDLG_H

#include <LayerGraphics.h>
#include <QRect>

#define SCALE_FACTOR 1

#include <gd.h>

class GDLG : public LayerGraphics
{
public:
    GDLG(bool a = false, QRect bR = QRect());
    ~GDLG();

    void writeFile(QString filename);

    void line(PointList pnts, qint64 w);
    void poly(PointList pnts, qint64 w);

private:
    gdImagePtr im;
    int black;
    int white;

    float xScaleFactor, yScaleFactor;
    int xShift, yShift;
};

#endif // GDLG_H
