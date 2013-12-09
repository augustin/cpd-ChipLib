#ifndef GRAPHICSLAYER_H
#define GRAPHICSLAYER_H

#include <qglobal.h>

class GraphicsLayer
{
public:
    virtual void rect(qint64 length, qint64 width, qint64 xpos, qint64 ypos, qint64 rotation = 0) = 0;
};

#endif // GRAPHICSLAYER_H
