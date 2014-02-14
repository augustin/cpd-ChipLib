#include <QtGui/QApplication>
#include <QFile>
#include <QFileInfo>
#include <QElapsedTimer>
#include <QTime>
#include <QPainter>
#include <QImage>
#include <math.h>

#include <Chip.h>
#include "../ChipDisplay/PainterLG.h"

#define qPrintableTime(ms) qPrintable(QTime(0, 0, 0, ms).toString("mm:ss.zzz"))

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	for(int i = 1; i < argc; i++) {
		QString arg(argv[i]);
		if(QFile::exists(arg)) {
			QElapsedTimer t;
			t.start();

			Chip* c = new Chip;
			c->load(arg);
			qDebug("[%s] Loaded chip %s.", qPrintableTime(t.elapsed()), qPrintable(QFileInfo(arg).fileName()));

			QRect bR = c->boundingRect();
			QImage img(abs(bR.width()/SCALE_FACTOR), abs(bR.height()/SCALE_FACTOR), QImage::Format_MonoLSB);
			img.fill(QColor(Qt::white).toRgb());
			QPainter p;
			qDebug("[%s] Created image object with size %dx%d.", qPrintableTime(t.elapsed()), abs(bR.width()/SCALE_FACTOR), abs(bR.height()/SCALE_FACTOR));

			p.begin(&img);
			c->render(new PainterLG(&p, true, bR), "");
			p.end();
			qDebug("[%s] Rendered.", qPrintableTime(t.elapsed()));

			img.save(arg + ".bmp");
			qDebug("[%s] Saved %s.\n", qPrintableTime(t.elapsed()), qPrintable(arg + ".png"));
		}
	}

	return 0;
}
