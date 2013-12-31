#ifndef CHIP_H
#define CHIP_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QList>

#include "LayerGraphics.h"

enum ObjType {
    RECTANGLE,
    LINE
};

struct ChipObject {
    ObjType type; /* all */
    qint64 x, y, w, l; /* RECT */
    PointList points; /* WIRE, POLYGON */
};

class ChipLayer : public QList<ChipObject*>
{
public:
    inline ChipLayer(QString name = "") : QList<ChipObject*>() { layerName = name; }

    inline QString name() { return layerName; }

    ChipObject* addRect(qint64 length, qint64 width, qint64 xpos, qint64 ypos);
    ChipObject* addLine(PointList points, qint64 width);

private:
    QString layerName;
};

class Chip
{
public:
    Chip();

    void load(QString fileName);
    ChipLayer* layer(QString name);

    QMap<QString, qint64> countObjs();
    void render(LayerGraphics* lg, QString layer = "", qint64 l = 0);
    QStringList layers;

private:
    QMap<QString, ChipLayer*> objectsForLayers;
};

#endif // CHIP_H
