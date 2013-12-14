#ifndef CIFREAD_H
#define CIFREAD_H

class QGraphicsView;
class QString;
class QByteArray;

#define LOG(type, line, desc) qDebug("%s: %d\t%s", type, line, qPrintable(desc))

QByteArray qFileGetContents(QString fileName);

#endif // CIFREAD_H
