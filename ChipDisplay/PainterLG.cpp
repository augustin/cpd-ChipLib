#include "PainterLG.h"

#include <QPainter>

PainterLG::PainterLG(QPainter *painter)
{
    p = painter;

}

void PainterLG::rect(qint64 length, qint64 width, qint64 xpos, qint64 ypos, qint64 rotation)
{
	p->drawRect(xpos, ypos, length, width);
}
