#ifndef GRAPHICSLAYERVIEW_H
#define GRAPHICSLAYERVIEW_H

#include <GraphicsLayer.h>
#include <QGraphicsScene>

class GraphicsLayerView : public GraphicsLayer
{
public:
    GraphicsLayerView(QGraphicsScene* g = 0);
    virtual void rect(qint64 length, qint64 width, qint64 xpos, qint64 ypos, qint64 rotation = 0);

private:
    QGraphicsScene* gs;
};

#endif // GRAPHICSLAYERVIEW_H
