#ifndef CHIP_H
#define CHIP_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QList>

class LayerGraphics;

struct ChipObject {
	qint64 x, y, w, l, r;
};

class Chip
{
public:
	Chip();

	void load(QString fileName);

	void addRect(QString layer, qint64 length, qint64 width, qint64 xpos, qint64 ypos, qint64 rotation = 0);

	QMap<QString, qint64> countObjs();
	void render(LayerGraphics* lg, QString layer = "", qint64 l = 0);

private:
	QStringList layers;
	QMap<QString, QList<ChipObject>* > objectsForLayers;
};

#endif // CHIP_H
