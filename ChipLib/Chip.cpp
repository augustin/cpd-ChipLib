#include "Chip.h"

#include "IO/Cif.h"

ChipObject* ChipLayer::addRect(qint64 length, qint64 width, qint64 xpos, qint64 ypos, qint64 rotation)
{
    ChipObject* c = new ChipObject;
    c->type = RECTANGLE;
    if(rotation != 0) {
        //int rot = rotation / 90;
        LOG("WARN", -1, "Rotations are unsupported (yet)!");
    } else {
        c->x = xpos;
        c->y = ypos;
        c->w = width;
        c->l = length;
    }
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

QMap<QString, qint64> Chip::countObjs()
{
    QMap<QString, qint64> ret;
    foreach(QString key, objectsForLayers.keys()) {
        ret.insert(key, objectsForLayers.value(key)->size());
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
        if(c->type == RECTANGLE) {
            lg->rect(c->l, c->w, c->x, c->y);
        } else if(c->type == LINE) {
            lg->line(c->points);
        }
    }
}
