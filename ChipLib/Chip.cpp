#include "Chip.h"

#include <IO/Cif.h>
#include "LayerGraphics.h"

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

void Chip::addRect(QString layer, qint64 length, qint64 width, qint64 xpos, qint64 ypos, qint64 rotation)
{
	if(!layers.contains(layer)) { layers.append(layer); }
	if(!objectsForLayers.contains(layer)) { objectsForLayers.insert(layer, new QList<ChipObject>); }
	ChipObject c;
	c.x = xpos;
	c.y = ypos;
	c.w = width;
	c.l = length;
	c.r = rotation;
	objectsForLayers.value(layer)->append(c);
}

void Chip::render(LayerGraphics *lg, QString layer, qint64 l)
{
	QList<ChipObject>* objects;
	if(layer.size()) { objects = objectsForLayers.value(layer); }
	else {
		objects = new QList<ChipObject>;
		foreach(QList<ChipObject>* a, objectsForLayers.values()) {
			for(int i = 0; i < a->size(); i++) {
				objects->append(a->at(i));
			}
		}
	}

	qint64 limit = l ? l : objects->size();
	for(qint64 i = 0; i < limit; i++) {
		ChipObject c = objects->at(i);
		lg->rect(c.l, c.w, c.x, c.y, c.r);
	}
}
