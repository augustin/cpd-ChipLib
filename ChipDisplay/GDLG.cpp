#include "GDLG.h"

#include <gd.h>
#include <QString>
#include <QFile>

GDLG::GDLG(bool, QRect bR)
{
    im = gdImageCreate(abs(bR.width()/SCALE_FACTOR), abs(bR.height()/SCALE_FACTOR));
    white = gdImageColorAllocate(im, 255, 255, 255);
    black = gdImageColorAllocate(im, 0, 0, 0);

    xScaleFactor = 1.0/SCALE_FACTOR;
    yScaleFactor = -1.0/SCALE_FACTOR;
    xShift = -bR.left();
    yShift = -bR.top()+1;
}

GDLG::~GDLG()
{
    gdImageDestroy(im);
}

#define X(x) (((x)*xScaleFactor)+xShift)
#define Y(y) (((y)*yScaleFactor)-yShift)

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

