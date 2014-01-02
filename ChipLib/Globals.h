#ifndef CHIPLIB_GLOBALS_H
#define CHIPLIB_GLOBALS_H

class QGraphicsView;
class QString;
class QByteArray;

#define CHIPLIB_VER "0.4.0"
#define LOG(type, line, desc) qDebug("%s:\t%d\t%s", type, line, qPrintable(desc))

QByteArray qFileGetContents(QString fileName);

#endif // CHIPLIB_GLOBALS_H
