#ifndef PAINTERLG_H
#define PAINTERLG_H

#include <LayerGraphics.h>

class QPainter;

class PainterLG : public LayerGraphics
{
public:
    PainterLG(QPainter* painter = 0);

    virtual void rect(qint64 length, qint64 width, qint64 xpos, qint64 ypos, qint64 rotation = 0);

private:
    QPainter* p;
};

#endif // PAINTERLG_H