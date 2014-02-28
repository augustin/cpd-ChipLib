#ifndef GDLG_H
#define GDLG_H

#include <LayerGraphics.h>
#include <QRect>

#include <gd.h>

struct PixelData {
    char* pixels;
    int width;
    int height;
};

class GDLG : public LayerGraphics
{
public:
    GDLG(QRect bR = QRect(), int scaleFactor = 1);
    ~GDLG();

    PixelData getPixels();
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
