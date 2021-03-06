#ifndef CHIP_H
#define CHIP_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QList>
#include <QRect>

#include "LayerGraphics.h"

enum ObjType {
    LINE, /* open line */
    POLYGON, /* closed line */
    RECTANGLE /* rectangle */
};

struct ChipObject {
    ObjType type;
    qint64 w; /* line/polygon */
    PointList points;
};

class ChipLayer : public QList<ChipObject*>
{
public:
    inline ChipLayer(QString name = "") : QList<ChipObject*>() { layerName = name; }

    inline QString name() { return layerName; }

    ChipObject* addRect(qint64 width, qint64 height, qint64 xpos, qint64 ypos);
    ChipObject* addLine(PointList points, qint64 width);
    ChipObject* addPoly(PointList points);

private:
    QString layerName;
};

class Chip
{
public:
    Chip();
    ~Chip();

    static QStringList supportedFormats();

    void load(QString fileName);
    ChipLayer* layer(QString name);

    QList<ChipObject*>* getObjects(QString layer);
    QList<ChipObject*>* getAllObjects();

    QMap<qint64, QString> countObjs();

	QRect boundingRect(QString layer = "");
    void render(LayerGraphics* lg, QString layer = "", qint64 l = 0);

    QStringList layers;

private:
    QMap<QString, ChipLayer*> objectsForLayers;
};

#endif // CHIP_H
