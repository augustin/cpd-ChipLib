#include "GDLG.h"
#include "Globals.h"

#include <gd.h>
#include <QString>
#include <QFile>
#include <QElapsedTimer>

GDLG::GDLG(QRect bR, int scaleFactor)
{
    ensureLog();
    QElapsedTimer t;
    t.start();

    im = gdImageCreate(abs(bR.width()/scaleFactor), abs(bR.height()/scaleFactor));
    white = gdImageColorAllocate(im, 255, 255, 255);
    black = gdImageColorAllocate(im, 0, 0, 0);

    xScaleFactor = 1.0/scaleFactor;
    yScaleFactor = -1.0/scaleFactor;
    xShift = -bR.left();
    yShift = -bR.top()+1;

    qint64 el = t.elapsed();
    writeLog(QString("I-003: GDLG took %1 ms.").arg(el));
}

GDLG::~GDLG()
{
    gdImageDestroy(im);
}

#define X(x) (((x)+xShift)*xScaleFactor)
#define Y(y) (((y)+yShift)*yScaleFactor)

void GDLG::line(PointList pnts, qint64 w)
{
    gdPoint points[pnts.size()];
    for(int i = 0; i < pnts.size(); i++) {
        Point* pnt = pnts.at(i);
        points[i].x = X(pnt->x);
        points[i].y = Y(pnt->y);
    }

    gdImageSetThickness(im, w);
    gdImageOpenPolygon(im, points, pnts.size(), black);
}

void GDLG::poly(PointList pnts, qint64 w)
{
    gdPoint points[pnts.size()+1];
    for(int i = 0; i < pnts.size(); i++) {
        Point* pnt = pnts.at(i);
        points[i].x = X(pnt->x);
        points[i].y = Y(pnt->y);
    }

    points[pnts.size()].x = X(pnts[0]->x);
    points[pnts.size()].y = Y(pnts[0]->y);

    gdImageSetThickness(im, w);
    gdImageFilledPolygon(im, points, pnts.size()+1, black);
}

PixelData GDLG::getPixels()
{
    PixelData ret;
    ret.width = im->sx;
    ret.height = im->sy;
    ret.pixels = (char*)malloc(im->sx*im->sy);

    int y = 0;
    while(y < im->sy) {
        memcpy((void*)(ret.pixels[ret.width*y]), (void*)(im->pixels[y]), im->sx);
        y++;
    }
    return ret;
}

void GDLG::writeFile(QString filename)
{
    qDebug("w:%d h:%d", im->sx, im->sy);

    int y = 0;
    QFile f(filename);
    f.open(QFile::WriteOnly);
    while(y < im->sy) {
        f.write((char*)(im->pixels[y]), im->sx);
        //f.write("\n");
        y++;
    }
    f.close();
}
