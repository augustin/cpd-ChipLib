#include "Chip.h"

#include "IO/Cif.h"

ChipObject* ChipLayer::addRect(qint64 length, qint64 width, qint64 x, qint64 y)
{
    qint64 xpos = x-(length/2), ypos = y-(width/2);
    ChipObject* c = new ChipObject;
    c->type = POLYGON;
    c->points.append(new Point(xpos, ypos));
    c->points.append(new Point(xpos+length, ypos));
    c->points.append(new Point(xpos+length, ypos+width));
    c->points.append(new Point(xpos, ypos+width));
    c->w = 1;
    append(c);
    return c;
}
ChipObject* ChipLayer::addLine(PointList points, qint64 width)
{
    ChipObject* c = new ChipObject;
    c->type = LINE;
    c->points.append(points);
    c->w = width;
    append(c);
    return c;
}
ChipObject* ChipLayer::addPoly(PointList points)
{
    ChipObject* c = new ChipObject;
    c->type = POLYGON;
    c->points.append(points);
    c->w = 1;
    append(c);
    return c;
}

Chip::Chip()
{
}

void Chip::load(QString fileName)
{
    if(fileName.endsWith("cif", Qt::CaseInsensitive)) {
        Cif::File* f = Cif::Reader::Read(qFileGetContents(fileName));
        Cif::Interpreter::run(this, f);
        delete f;
    }
}

ChipLayer* Chip::layer(QString name)
{
    if(!layers.contains(name)) {
        layers.append(name);
        ChipLayer* newLayer = new ChipLayer(name);
        objectsForLayers.insert(name, newLayer);
        return newLayer;
    } else {
        return objectsForLayers.value(name);
    }
}

QMap<qint64, QString> Chip::countObjs()
{
    QMap<qint64, QString> ret;
    foreach(QString key, objectsForLayers.keys()) {
        ret.insert(objectsForLayers.value(key)->size(), key);
    }
    return ret;
}

void Chip::render(LayerGraphics *lg, QString layer, qint64 l)
{
    QList<ChipObject*>* objects;
    if(layer.size()) {
        objects = objectsForLayers.value(layer);
    } else {
        objects = new QList<ChipObject*>;
        foreach(QList<ChipObject*>* a, objectsForLayers.values()) {
            for(int i = 0; i < a->size(); i++) {
                objects->append(a->at(i));
            }
        }
    }

    qint64 limit; // =  ? l : objects->size();
    if(l < 1) { l = objects->size(); }
    if(objects->size() > l) {
        limit = l;
    } else {
        limit = objects->size();
    }
    for(qint64 i = 0; i < limit; i++) {
        ChipObject* c = objects->at(i);
        if(c->type == LINE) {
            lg->line(c->points, c->w);
        } else if(c->type == POLYGON) {
            lg->poly(c->points, c->w);
        }
    }
}
